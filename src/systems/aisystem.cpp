// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/LuaAPI.hpp>
#include <NDK/Lua/LuaBinding_SDK.hpp>
#include <Nazara/Lua/LuaClass.hpp>
#include "components/common/pathcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/movecomponent.hpp"
#include "components/common/fightcomponent.hpp"
#include "components/common/lifecomponent.hpp"
#include "components/common/levelcomponent.hpp"
#include "components/common/damagemodifiercomponent.hpp"
#include "util/aiutil.hpp"
#include "util/nzstlcompatibility.hpp"
#include "util/maputil.hpp"
#include "def/gamedef.hpp"
#include "def/systemdef.hpp"
#include "data/elementdata.hpp"
#include "systems/aisystem.hpp"

AISystem::AISystem(const SkillStore& skills, const AICore& ais, const Nz::String& utilFilepath, const std::shared_ptr<micropather::MicroPather>& pather, const Ndk::EntityHandle& mainCharacter)
    : m_mainCharacter { mainCharacter }, m_utilityLuaFile { utilFilepath }, m_skills(skills), m_ais(ais)
{
    Requires<PositionComponent>();
    SetUpdateOrder(Def::AISystemUpdateOrder);
    setPather(pather);
}

AISystem::AISystem(const AISystem& other) : m_skills (other.m_skills), m_ais(other.m_ais) // Invalid ctor. Must not be used
{
    TealAssert(false, "AISystem's copy constructor cannot be used");
    // This constructor is only provided to remove an error
}

void AISystem::reset()
{
    TealAssert(m_pather, "Pather is null, cannot reset it !");
    m_pather->Reset();
}

void AISystem::setPather(const std::shared_ptr<micropather::MicroPather>& pather)
{
    m_pather = pather;
}

void AISystem::OnUpdate(float elapsed)
{
    for (auto& e : GetEntities())
    {
        if (e->HasComponent<MoveComponent>() && e->HasComponent<PathComponent>()) // Compute Path
        {
            auto& move = e->GetComponent<MoveComponent>();

            if (move.tile != toVector2(Def::StandStillPos)) // This entity wants to move
            {
                auto& pos = e->GetComponent<PositionComponent>();
                auto& pathComp = e->GetComponent<PathComponent>();
                auto& path = pathComp.path;

                if (move.tile == pos.xy) // Check if user clicked on the tile he is
                {
                    move.tile = toVector2(Def::StandStillPos);
                    continue;
                }

                auto currentPath = directionsToPositions(path, pos.xy);

                if (!currentPath.empty() && move.tile == currentPath.back() && move.playerInitiated) // If user clicked to go to the same location, stop.
                {                                                                                    // Else, recompute path in case an object moved and blocks the path
                    move.tile = toVector2(Def::StandStillPos);                                        // (See MovementSystem)
                    continue;
                }

                PathComponent::PathPool newPath;

                if (pos.advancement == 0)
                    newPath = computePath(e, m_pather.get());
                else
                    continue;

                if (!newPath.empty())
                {
                    path = newPath;
                    pos.advancement = 0;
                }

                /// \todo if !move.playerInitiated and no path found (something blocked and still blocking)
                ///       - stand still if valid pos
                ///       - or use a function to go to nearest valid tile

                move.tile = toVector2(Def::StandStillPos);
            }
        }

        /*
        Todo: create another component (or use fight component ?) & system (FightSystem. such original) for fight
        With a queue, like path queue (so FightSystem will work like MovementSystem: execute actions)
        queue will have things like:
         - Move Up|Left
         - Attack with Legendary Sword
         - Move Right
        Path will be mixed with attacks, so it is necessary
        to have a new component/system to keep the order
        use movementutil for move functions, no need to use move system
        */

        // All swords/hammers/any offensive item have an attack
        // create an OffensiveComponent ?

        if (e->HasComponent<FightComponent>() && e->HasComponent<LifeComponent>() && e->HasComponent<MonsterComponent>() && e != m_mainCharacter) // Compute fight
        {
            auto& fight = e->GetComponent<FightComponent>();
            auto& life = e->GetComponent<LifeComponent>();
            auto& monster = e->GetComponent<MonsterComponent>();

            if (fight.isFighting && fight.myTurn) // Time to act !
            {
                TealAssert(m_isFightActive, "Not fighting");
                Nz::LuaInstance& lua = m_currentFight.ai;

                if (!m_currentFight.coroutine)
                {
                    m_currentFight.forceContinueFight = [] () { return false; };
                    m_currentFight.currentEntity = e;
                    lua.SetTimeLimit(Def::LuaAITimeLimit);

                    if (!prepareLuaAI(lua))
                    {
                        NazaraError("Failed to prepare Lua AI");

                        cleanAndContinueFight();
                        continue;
                    }

                    AICore::TagInfo aiInfo = m_ais.getTagInfoFromTagKeys(std::make_pair(monster.family, monster.name));
                    Nz::String& aiName = monster.name;

                    m_currentFight.coroutine.reset(new Nz::LuaCoroutine { std::move(lua.NewCoroutine()) });
                    m_currentFight.coroutine->GetGlobal("execute");

                    Nz::Ternary result = m_currentFight.coroutine->Resume();

                    if (result != Nz::Ternary_Unknown) // yield() wasn't called. AI did nothing.
                    {
                        cleanAndContinueFight();

                        if (result == Nz::Ternary_False)
                            NazaraError("An error occurred in Fight AI \"" + aiName + "\"");
                    }
                }

                else // yield() was called. Resume execution.
                {
                    if (m_currentFight.coroutine->CanResume() && !m_currentFight.forceContinueFight())
                    {
                        if (!m_currentFight.canResume()) // Action isn't finished
                            continue;

                        m_currentFight.canResume = [] () { return true; };
                        Nz::String aiName = lua.CheckGlobal<Nz::String>("fight_ai_name");
                        Nz::Ternary result = m_currentFight.coroutine->Resume();

                        if (result != Nz::Ternary_Unknown)
                        {
                            cleanAndContinueFight();

                            if (result == Nz::Ternary_False)
                                NazaraError("An error occurred in Fight AI \"" + aiName + "\"");
                        }
                    }

                    else
                    {
                        m_currentFight.forceContinueFight = [] () { return false; };
                        cleanAndContinueFight();
                    }
                }
            }
        }
    }
}

void AISystem::cleanAndContinueFight()
{
    cleanLuaInstance(m_currentFight.ai);
    m_currentFight.currentEntity->GetComponent<FightComponent>().myTurn = false;

    auto it = std::find(m_currentFight.fighters.begin(), m_currentFight.fighters.end(), m_currentFight.nextEntity);
    TealException(it != m_currentFight.fighters.end(), "Entity not found");

    m_currentFight.currentEntity = m_currentFight.nextEntity;

    if (it == (m_currentFight.fighters.end() - 1))
        m_currentFight.nextEntity = m_currentFight.fighters[0];

    else
        m_currentFight.nextEntity = *(it + 1);
}

void AISystem::cleanLuaInstance(Nz::LuaInstance& lua)
{
    lua.PushNil(); lua.SetGlobal("teal_fight_data");
    lua.PushNil(); lua.SetGlobal("execute");
    lua.PushNil(); lua.SetGlobal("fight_ai_name");
    lua.PushNil(); lua.SetGlobal("fight_ai_monster_family");
    lua.PushNil(); lua.SetGlobal("fight_ai_monster_name");

    m_currentFight.coroutine = nullptr;
}

void AISystem::removeFromFight(const Ndk::EntityHandle& e) // Always check forceContinueFight after using this function
{
    auto& fight = e->GetComponent<FightComponent>();
    fight.isFighting = false;
    fight.myTurn = false;

    auto it = std::find(m_currentFight.fighters.begin(), m_currentFight.fighters.end(), e);
    if (it != m_currentFight.fighters.end())
        m_currentFight.fighters.erase(it);


    if (m_currentFight.nextEntity == e)
    {
        if (it == (m_currentFight.fighters.end() - 1))
            m_currentFight.nextEntity = m_currentFight.fighters[0];

        else
            m_currentFight.nextEntity = *(it + 1);
    }

    if (m_currentFight.currentEntity == e)
    {
        cleanAndContinueFight();
        m_currentFight.forceContinueFight = [] () { return true; };
    }
}

bool AISystem::prepareLuaAI(Nz::LuaInstance& lua)
{
    lua.ExecuteFromFile(m_utilityLuaFile);
    lua.PushTable();
    {
        lua.PushTable();

        if (!bindCharacter(lua, m_currentFight.currentEntity))
            return false;

        lua.SetMetatable("CurrentCharacter");
        lua.SetField("character");

        {
            lua.PushTable();

            for (unsigned i {}; i < m_currentFight.fighters.size(); ++i)
            {
                auto& e = m_currentFight.fighters[i];

                if (e == m_currentFight.currentEntity)
                    continue;

                lua.PushInteger(i + 1);
                lua.PushTable();

                if (!bindCharacter(lua, e))
                    return false;

                lua.SetTable();
            }

            lua.SetField("characters");


            lua.PushTable();

            for (auto& e : m_currentFight.entities)
            {
                // bind traps and things like that later...
            }

            lua.SetField("objects");
        }
    }

    lua.SetGlobal("teal_fight_data");

    bindFunctions(lua);
    return true;
}

void AISystem::bindFunctions(Nz::LuaInstance& lua)
{
    Ndk::LuaAPI::RegisterClasses(lua); // TODO: since all components are bound using Entity bind thing, delete componentName() in components

    Nz::LuaClass<AISystem*> thisClass;
    thisClass.Reset("TealClass");

    thisClass.BindMethod("MoveCharacter",          &AISystem::Teal_MoveCharacter);
    thisClass.BindMethod("TakeCover",              &AISystem::Teal_TakeCover);
    thisClass.BindMethod("AttackCharacter",        &AISystem::Teal_AttackCharacter);
    thisClass.BindMethod("MoveAndAttackCharacter", &AISystem::Teal_MoveAndAttackCharacter);
    thisClass.BindMethod("ChooseTarget",           &AISystem::Teal_ChooseTarget);
    thisClass.BindMethod("ChooseAttack",           &AISystem::Teal_ChooseAttack);
    thisClass.BindMethod("CanAttack",              &AISystem::Teal_CanAttack);
    thisClass.BindMethod("CanAttackWith",          &AISystem::Teal_CanAttackWith);

    thisClass.Register(lua);

    lua.PushInstance("TealClass", this);
    lua.SetGlobal("Teal");
}

bool AISystem::bindCharacter(Nz::LuaInstance& lua, const Ndk::EntityHandle& character, bool skills)
{


    /*bool somethingWentWrong = false;

    {
        auto it = std::find_if(m_currentFight.fighters.begin(), m_currentFight.fighters.end(), [&character] (const Ndk::EntityHandle& e)
        {
            return e == character;
        });

        if (it == m_currentFight.fighters.end())
        {
            lua.PushInteger(-1); // well fuck
            somethingWentWrong = true;
        }

        else
            lua.PushInteger(std::distance(m_currentFight.fighters.begin(), it));

        lua.SetField("index");
    }

    {
        auto& pos = character->GetComponent<PositionComponent>().xy;

        lua.PushInteger(pos.x);
        lua.SetField("x");

        lua.PushInteger(pos.y);
        lua.SetField("y");
    }

    lua.PushInteger(character->GetComponent<LevelComponent>().level);
    lua.SetField("level");

    {
        auto& fight = character->GetComponent<FightComponent>();

        lua.PushInteger(fight.maxMovementPoints);
        lua.SetField("mp");

        lua.PushInteger(fight.maxActionPoints);
        lua.SetField("ap");

        auto& life = character->GetComponent<LifeComponent>();

        lua.PushInteger(life.hp);
        lua.SetField("hp");

        lua.PushInteger(life.maxHp);
        lua.SetField("maxHp");
    }

    {
        { // Attack Modifier
            lua.PushTable(0u, 5u);
            auto& atk = character->GetComponent<AttackModifierComponent>().data;

            for (EnumUnderlyingType<Element> i {}; i < toUnderlyingType(Element::Max); ++i)
            {
                Element element = static_cast<Element>(i);
                lua.PushTable(2u);

                {
                    lua.PushInteger(1);
                    lua.PushString(elementToString(element));
                    lua.SetTable();

                    lua.PushInteger(2);
                    lua.PushInteger(atk[element]);
                    lua.SetTable();
                }

                lua.PushInteger(i + 1);
                lua.SetTable();
            }

            lua.SetField("attackModifier");
        }

        { // Resistance modifier
            lua.PushTable(0u, 5u);
            auto& res = character->GetComponent<ResistanceModifierComponent>().data;

            for (EnumUnderlyingType<Element> i {}; i < toUnderlyingType(Element::Max); ++i)
            {
                Element element = static_cast<Element>(i);
                lua.PushTable(2u);

                {
                    lua.PushInteger(1);
                    lua.PushString(elementToString(element));
                    lua.SetTable();

                    lua.PushInteger(2);
                    lua.PushInteger(res[element]);
                    lua.SetTable();
                }

                lua.PushInteger(i + 1);
                lua.SetTable();
            }

            lua.SetField("resistanceModifier");
        }
    }

    if (skills)
    {
        lua.PushTable();

        if (!serializeSkills(lua, character))
            return false;

        lua.SetField("skills");
    }

    return !somethingWentWrong;*/
}

bool AISystem::bindSkills(Nz::LuaInstance& lua, const Ndk::EntityHandle& character)
{
    /*auto& fight = character->GetComponent<FightComponent>();

    for (auto& skillId : fight.attacks)
    {
        auto& skill = m_skills.getItem(skillId);

        {
            lua.PushField("mp", skill.movementPoints);
            lua.PushField("ap", skill.actionPoints);
        }

        {
            lua.PushField("minRange", skill.minRange);
            lua.PushField("maxRange", skill.maxRange);
            lua.PushField("modifiableRange", skill.modifiableRange);
        }

        {
            lua.PushField("areatype", SkillData::areaTypeToString(skill.areaType));
            lua.PushField("areaMinRange", skill.areaMinRange);
            lua.PushField("areaMaxRange", skill.areaMaxRange);
        }

        lua.PushField("codename", skill.codename);

        lua.PushTable();

        for (unsigned i {}; i < skill.attackEffects.size(); ++i)
        {
            auto& attackPair = skill.attackEffects[i];
            auto& attack = attackPair.second;

            TealAssert(attack.get(), "Attack nullptr");

            lua.PushInteger(i + 1);
            lua.PushTable();

            {
                lua.PushString(AttackData::targetToString(attack->data.target));
                lua.SetField("target");

                switch (attackPair.first)
                {
                    case SkillData::AttackType::Damage:
                    { //< A new scope is here needed to remove the error "initialization of 'dmg' is skipped by 'case' label"
                        DamageData* dmg = static_cast<DamageData*>(attack.get());

                        lua.PushString(elementToString(dmg->damage.first));
                        lua.SetField("element");

                        lua.PushInteger(dmg->damage.second);
                        lua.SetField("damage");

                        lua.PushString("damage");
                        break;
                    }

                    case SkillData::AttackType::State:
                    {
                        StateData* state = static_cast<StateData*>(attack.get());

                        // TODO
                        NazaraError("State case has not been implemented yet. Have a good crash!");

                        lua.PushString("state");
                        break;
                    }

                    case SkillData::AttackType::Effect:
                    {
                        EffectData* effect = static_cast<EffectData*>(attack.get());

                        // TODO
                        NazaraError("Effect case has not been implemented yet. Wanna crash again?");

                        lua.PushString("effect");
                        break;
                    }

                    default:
                        lua.PushString("");
                        break;
                }

                lua.SetField("type");
            }

            lua.SetTable();
        }

        lua.SetField("attacks");
    }

    return true;*/
}


void AISystem::Teal_MoveCharacter(unsigned x, unsigned y)
{
    TealAssert(m_isFightActive, "Not fighting");
    auto& move = m_currentFight.currentEntity->GetComponent<MoveComponent>();

    move.tile = { x, y };
    move.playerInitiated = true; // Actually it's not player initiated but it will make sure the MovementSystem
                                 // won't try to recompute the path. It's a fight... | todo: change variable name ?

    m_currentFight.canResume = [this, x, y] ()
    {
        return m_currentFight.currentEntity->GetComponent<PositionComponent>().xy == AbsTile { x, y };
    };
}

void AISystem::Teal_TakeCover()
{
    TealAssert(m_isFightActive, "Not fighting");

    auto& me = m_currentFight.currentEntity;
    auto& fight = me->GetComponent<FightComponent>();
    auto& pos = me->GetComponent<PositionComponent>();

    std::vector<AbsTile> possibleTiles; // Possible tiles to go to
    std::vector<AbsTile> visibleTiles = getVisibleTiles(pos.xy, fight.movementPoints);

    for (unsigned i {}; i < visibleTiles.size(); ++i)
    {
        /*AbsTile difference = distance(toVector(IndexToXY(i)), pos.xy);
        const TileData& tile = getCurrentMap()->getCurrentMap()->tiles()[i];*/

        std::vector<AbsTile> path = directionsToPositions(computePath(pos.xy, toVector2(IndexToXY(i)), m_pather.get()), pos.xy);

        if (path.size() <= fight.movementPoints)
            possibleTiles.push_back(path.back());
    }

    auto& res = me->GetComponent<ResistanceModifierComponent>();
    Ndk::EntityList enemies = getEnemies(me);

    AbsTile recommendedTile { toVector2(Def::StandStillPos) };
    unsigned minDamage { std::numeric_limits<unsigned>().max() };

    for (const auto& tile : possibleTiles)
    {
        unsigned maxDamagePerTile {};

        for (const auto& enemy : enemies)
        {
            auto& eAttack = enemy->GetComponent<AttackModifierComponent>();
            auto& eFight = enemy->GetComponent<FightComponent>();
            auto& ePos = enemy->GetComponent<PositionComponent>();

            unsigned maxDamagePerEnemy {};

            for (SkillStore::LightId attackId : eFight.attacks)
            {
                const SkillData& skill = m_skills.getItem(attackId);
                auto damage = getMaximumDamage(ePos.xy, tile, skill);
                unsigned maxDamagePerSkill {};

                for (Element i {}; i <= Element::Max; ++i)
                {
                    unsigned resistancePercent = res.data[i];

                    if (resistancePercent >= 100)
                        continue;

                    unsigned rawDamage = damage[i];
                    unsigned resistanceDamage = rawDamage / 100 * resistancePercent;

                    if (resistanceDamage >= rawDamage)
                        continue;

                    unsigned calculatedDamage = rawDamage - resistanceDamage;
                    maxDamagePerSkill = std::max(maxDamagePerSkill, calculatedDamage);
                }

                maxDamagePerEnemy = std::max(maxDamagePerEnemy, maxDamagePerSkill);
            }

            maxDamagePerTile = std::max(maxDamagePerTile, maxDamagePerEnemy);
        }

        if (maxDamagePerTile < minDamage)
        {
            minDamage = maxDamagePerTile;
            recommendedTile = tile;
        }
    }

    if (recommendedTile != toVector2(Def::StandStillPos))
        Teal_MoveCharacter(recommendedTile.x, recommendedTile.y);
}

void AISystem::Teal_AttackCharacter(unsigned characterIndex, Nz::String skillCodename)
{
    TealAssert(m_isFightActive, "Not fighting");

    if (characterIndex > m_currentFight.fighters.size())
    {
        NazaraError("AI: Character out of bounds !");
        return;
    }

    auto& fight = m_currentFight.currentEntity->GetComponent<FightComponent>();
    fight.target = m_currentFight.fighters[characterIndex - 1];
    // todo: canResume function
}

void AISystem::Teal_MoveAndAttackCharacter(unsigned characterIndex, Nz::String skillCodename)
{
    TealAssert(m_isFightActive, "Not fighting");

    // Move near character...

    bool canAttack {};

    if (canAttack)
        Teal_AttackCharacter(characterIndex, skillCodename);
}

unsigned AISystem::Teal_ChooseTarget() // todo: raycasting ?
{
    TealAssert(m_isFightActive, "Not fighting");
    return 0;
}

unsigned AISystem::Teal_ChooseAttack(unsigned characterIndex)
{
    return ChooseAttack(characterIndex, m_currentFight.currentEntity->GetComponent<PositionComponent>().xy);
}

unsigned AISystem::ChooseAttack(unsigned characterIndex, const AbsTile& pos) // todo: raycasting ?
{
    TealAssert(m_isFightActive, "Not fighting");

    if (!Teal_CanAttack(characterIndex))
        return 0;

    if (characterIndex > m_currentFight.fighters.size())
    {
        NazaraError("AI: Character out of bounds !");
        return false;
    }

    Ndk::EntityHandle opponent = m_currentFight.fighters[characterIndex - 1];
    Ndk::EntityHandle me = m_currentFight.currentEntity;

    AbsTile opponentPos = opponent->GetComponent<PositionComponent>().xy;
    AbsTile myPos = me->GetComponent<PositionComponent>().xy;

    //...

    return 0;
}

bool AISystem::Teal_CanAttack(unsigned characterIndex) // todo: raycasting ?
{
    TealAssert(m_isFightActive, "Not fighting");

    if (characterIndex > m_currentFight.fighters.size())
    {
        NazaraError("AI: Character out of bounds !");
        return false;
    }
    
    Ndk::EntityHandle me = m_currentFight.currentEntity;
    auto& myFight = me->GetComponent<FightComponent>();
    
    for (unsigned skillIndex { 1 }; skillIndex < (myFight.attacks.size() + 1); ++skillIndex)
    {
        if (Teal_CanAttackWith(characterIndex, skillIndex))
            return true;
    }

    return false;
}

bool AISystem::Teal_CanAttackWith(unsigned characterIndex, unsigned skillIndex) // todo: raycasting ?
{
    TealAssert(m_isFightActive, "Not fighting");

    if (characterIndex > m_currentFight.fighters.size())
    {
        NazaraError("AI: Character out of bounds !");
        return false;
    }

    Ndk::EntityHandle opponent = m_currentFight.fighters[characterIndex - 1];
    AbsTile opponentPos = opponent->GetComponent<PositionComponent>().xy;
    auto& opponentFight = opponent->GetComponent<FightComponent>();

    Ndk::EntityHandle me = m_currentFight.currentEntity;
    AbsTile myPos = me->GetComponent<PositionComponent>().xy;
    auto& myFight = me->GetComponent<FightComponent>();

    if (skillIndex > myFight.attacks.size())
    {
        NazaraError("AI: Skill out of bounds !");
        return false;
    }

    const SkillStore::LightId skillId = myFight.attacks[skillIndex - 1];
    const SkillData& skill = m_skills.getItem(skillId);

    if (myFight.maxActionPoints < skill.actionPoints || myFight.maxMovementPoints < skill.movementPoints)
        return false;

    bool allyOnly { true };
    AttackData::Target attackTypeToAvoid { myFight.teamNumber == opponentFight.teamNumber ? AttackData::Target::Enemies : AttackData::Target::Allies };

    for (auto& attack : skill.attackEffects)
    {
        if (attack.second->data.target != attackTypeToAvoid)
        {
            allyOnly = false;
            break;
        }
    }

    if (allyOnly)
        return false;

    Nz::Vector2ui posDistance = { distance(myPos.x, opponentPos.x), distance(myPos.y, opponentPos.y) };

    if (posDistance.x < skill.minRange || posDistance.x > skill.maxRange ||
        posDistance.y < skill.minRange || posDistance.y > skill.maxRange)
        return false; // todo: check area min/max range & type, in case of "damage zone"

    if (skill.maxRange > 1 && !skill.viewThroughObstacles)
    {
        NazaraError("Raycasting has not been implemented (yet)");
        return false; // todo: implement raycasting thing here
    }

    return true;
}

std::unordered_map<Element, unsigned> AISystem::getMaximumDamage(const AbsTile& from, const AbsTile& target, const SkillData& skill)
{
    std::vector<AbsTile> possibleTilesToAttack = getVisibleTiles(from, skill.maxRange, skill.viewThroughObstacles);
    std::function<std::unordered_map<Element, unsigned>(const AbsTile& tile)> damageDone;
    NazaraError("implement this better");

    switch (skill.areaType) // Don't forget to do raycasting in case there's an obstacle
    {
        case SkillData::AreaType::Cross:
            damageDone = [&target, &skill] (const AbsTile& tile) -> std::unordered_map<Element, unsigned>
            {
                if (target != tile)
                    return {};

                return skill.getMaximumDamage();
            };
            break;

        case SkillData::AreaType::Plus:
            damageDone = [&target, &skill] (const AbsTile& tile) -> std::unordered_map<Element, unsigned>
            {
                if (target != tile)
                    return {};

                return skill.getMaximumDamage();
            };
            break;

        case SkillData::AreaType::Circle:
            damageDone = [&target, &skill] (const AbsTile& tile) -> std::unordered_map<Element, unsigned>
            {
                if (target != tile)
                    return {};

                return skill.getMaximumDamage();
            };
            break;

        case SkillData::AreaType::AlignedCenterRL:
            damageDone = [&target, &skill] (const AbsTile& tile) -> std::unordered_map<Element, unsigned>
            {
                if (target != tile)
                    return {};

                return skill.getMaximumDamage();
            };
            break;

        case SkillData::AreaType::AlignedCenterDU:
            damageDone = [&target, &skill] (const AbsTile& tile) -> std::unordered_map<Element, unsigned>
            {
                if (target != tile)
                    return {};

                return skill.getMaximumDamage();
            };
            break;

        case SkillData::AreaType::AlignedRight:
            damageDone = [&target, &skill] (const AbsTile& tile) -> std::unordered_map<Element, unsigned>
            {
                if (target != tile)
                    return {};

                return skill.getMaximumDamage();
            };
            break;

        case SkillData::AreaType::AlignedLeft:
            damageDone = [&target, &skill] (const AbsTile& tile) -> std::unordered_map<Element, unsigned>
            {
                if (target != tile)
                    return {};

                return skill.getMaximumDamage();
            };
            break;

        case SkillData::AreaType::AlignedDown:
            damageDone = [&target, &skill] (const AbsTile& tile) -> std::unordered_map<Element, unsigned>
            {
                if (target != tile)
                    return {};

                return skill.getMaximumDamage();
            };
            break;

        case SkillData::AreaType::AlignedUp:
            damageDone = [&target, &skill] (const AbsTile& tile) -> std::unordered_map<Element, unsigned>
            {
                if (target != tile)
                    return {};

                return skill.getMaximumDamage();
            };
            break;

        default:
            damageDone = [&target, &skill] (const AbsTile& tile) -> std::unordered_map<Element, unsigned>
            {
                if (target != tile)
                    return {};

                return skill.getMaximumDamage();
            };
            break;
    }

    return damageDone(target);
}

Ndk::EntityList AISystem::getEnemies(const Ndk::EntityHandle& e)
{
    TealAssert(m_isFightActive, "Not fighting");

    Ndk::EntityList enemies;
    auto& fight = e->GetComponent<FightComponent>();

    for (auto& entity : m_currentFight.fighters)
    {
        if (entity->GetComponent<FightComponent>().teamNumber != fight.teamNumber)
            enemies.Insert(entity);
    }

    return enemies;
}

Ndk::EntityList AISystem::getAllies(const Ndk::EntityHandle& e)
{
    TealAssert(m_isFightActive, "Not fighting");

    Ndk::EntityList allies;
    auto& fight = e->GetComponent<FightComponent>();

    for (auto& entity : m_currentFight.fighters)
    {
        if (entity->GetComponent<FightComponent>().teamNumber == fight.teamNumber)
            allies.Insert(entity);
    }

    return allies;
}
