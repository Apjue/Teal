// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/common/pathcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/movecomponent.hpp"
#include "components/common/fightcomponent.hpp"
#include "components/common/lifecomponent.hpp"
#include "components/common/levelcomponent.hpp"
#include "components/common/attackmodifiercomponent.hpp"
#include "components/common/resistancemodifiercomponent.hpp"
#include "util/aiutil.hpp"
#include "util/nzstlcompatibility.hpp"
#include "util/maputil.hpp"
#include "def/gamedef.hpp"
#include "def/systemdef.hpp"
#include "data/elementdata.hpp"
#include "systems/aisystem.hpp"

AISystem::AISystem(const SkillStore& skills, const Nz::String& utilFilepath, const std::shared_ptr<micropather::MicroPather>& pather, const Ndk::EntityHandle& mainCharacter)
    : m_mainCharacter { mainCharacter }, m_utilityLuaFile { utilFilepath }, m_skills(skills)
{
    Requires<PositionComponent>();
    SetUpdateOrder(Def::AISystemUpdateOrder);
    setPather(pather);
}

AISystem::AISystem(const AISystem& other) : m_skills (other.m_skills) // Invalid ctor. Shouldn't be used
{
    TealAssert(false, "AISystem's copy constructor cannot be used");
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

            if (move.tile != toVector(Def::StandStillPos)) // This entity wants to move
            {
                auto& pos = e->GetComponent<PositionComponent>();
                auto& pathComp = e->GetComponent<PathComponent>();
                auto& path = pathComp.path;

                if (move.tile == pos.xy) // Check if user clicked on the tile he is
                {
                    move.tile = toVector(Def::StandStillPos);
                    continue;
                }

                auto currentPath = directionsToPositions(path, pos.xy);

                if (!currentPath.empty() && move.tile == currentPath.back() && move.playerInitiated) // If user clicked to go to the same location, stop.
                {                                                                                    // Else, recompute path in case an object moved and blocks the path
                    move.tile = toVector(Def::StandStillPos);                                        // (See MovementSystem)
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

                move.tile = toVector(Def::StandStillPos);
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

        if (e->HasComponent<FightComponent>() && e->HasComponent<LifeComponent>() && e != m_mainCharacter) // Compute fight
        {
            auto& fight = e->GetComponent<FightComponent>();
            auto& life = e->GetComponent<LifeComponent>();

            if (fight.isFighting && fight.myTurn) // Time to act !
            {
                TealAssert(m_isFightActive, "Not fighting");
                Nz::LuaInstance& lua = m_currentFight.ai;

                if (!m_currentFight.coroutine)
                {
                    m_currentFight.currentEntity = e;
                    lua.SetTimeLimit(std::max(1, static_cast<int>(elapsed * 10.f)) * 1000);

                    if (!prepareLuaAI(lua))
                    {
                        NazaraError("Failed to prepare Lua AI");

                        cleanAndContinueFight();
                        continue;
                    }

                    // Choose proper AI here. AICore ?

                    Nz::String aiName = lua.CheckGlobal<Nz::String>("fight_ai_name");
                    Nz::String aiType = lua.CheckGlobal<Nz::String>("fight_ai_type");
                    Nz::String aiMonsterType = lua.CheckGlobal<Nz::String>("fight_ai_monstertype");

                    m_currentFight.coroutine = &(lua.NewCoroutine()); // crash ?
                    m_currentFight.coroutine->GetGlobal("execute");
                    Nz::Ternary result = m_currentFight.coroutine->Resume();

                    if (result != Nz::Ternary_Unknown) // yield() wasn't called. AI did nothing.
                    {
                        cleanAndContinueFight();

                        if (result == Nz::Ternary_False)
                            NazaraError("An error occurred in Fight AI " + aiName);
                    }
                }

                else // yield() was called. Resume execution.
                {
                    if (m_currentFight.coroutine->CanResume())
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
                                NazaraError("An error occurred in Fight AI " + aiName);
                        }
                    }

                    else
                    {
                        NazaraError("Cannot resume Fight AI coroutine");
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
}

void AISystem::cleanLuaInstance(Nz::LuaInstance& lua)
{
    lua.PushNil(); lua.SetGlobal("teal_fight_data");
    lua.PushNil(); lua.SetGlobal("execute");
    lua.PushNil(); lua.SetGlobal("fight_ai_name");
    lua.PushNil(); lua.SetGlobal("fight_ai_type");
    lua.PushNil(); lua.SetGlobal("fight_ai_monstertype");

    m_currentFight.coroutine = nullptr;
}

bool AISystem::prepareLuaAI(Nz::LuaInstance& lua)
{
    lua.ExecuteFromFile(m_utilityLuaFile);
    lua.PushTable();


    lua.PushTable();

    if (!serializeCharacter(lua, m_currentFight.currentEntity))
        return false;

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

            if (!serializeCharacter(lua, e))
                return false;

            lua.SetTable();
        }

        lua.SetField("characters");


        lua.PushTable();

        for (auto& e : m_currentFight.entities)
        {
            // serialize traps and things like that later...
        }

        lua.SetField("objects");
    }

    lua.SetGlobal("teal_fight_data");
    lua.Execute("setmetatable(teal_fight_data.character, Character)"); // todo: use lua.SetMetatable();

    return true;
}

bool AISystem::serializeCharacter(Nz::LuaInstance& lua, const Ndk::EntityHandle& character, bool skills)
{
    {
        auto it = std::find_if(m_currentFight.fighters.begin(), m_currentFight.fighters.end(),
                               [&character] (const Ndk::EntityHandle& e)
        {
            return e == character;
        });

        if (it == m_currentFight.fighters.end())
            lua.PushInteger(-1); // well fuck

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

        lua.PushInteger(fight.movementPoints);
        lua.SetField("mp");

        lua.PushInteger(fight.actionPoints);
        lua.SetField("ap");

        auto& life = character->GetComponent<LifeComponent>();

        lua.PushInteger(life.hp);
        lua.SetField("hp");

        lua.PushInteger(life.maxhp);
        lua.SetField("maxhp");
    }

    {
        if (character->HasComponent<AttackModifierComponent>())
        {
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


        if (character->HasComponent<ResistanceModifierComponent>())
        {
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

    return true;
}

bool AISystem::serializeSkills(Nz::LuaInstance& lua, const Ndk::EntityHandle& character)
{
    auto& fight = character->GetComponent<FightComponent>();

    for (auto& skillId : fight.attacks)
    {
        auto& skill = m_skills.getItem(skillId);

        {
            lua.PushInteger(skill.movementPoints);
            lua.SetField("mp");

            lua.PushInteger(skill.actionPoints);
            lua.SetField("ap");
        }

        {
            lua.PushInteger(skill.minRange);
            lua.SetField("minRange");

            lua.PushInteger(skill.maxRange);
            lua.SetField("maxRange");

            lua.PushBoolean(skill.modifiableRange);
            lua.SetField("modifiableRange");
        }

        {
            lua.PushString(SkillData::areaTypeToString(skill.areaType));
            lua.SetField("areatype");

            lua.PushInteger(skill.areaMinRange);
            lua.SetField("areaMinRange");

            lua.PushInteger(skill.areaMaxRange);
            lua.SetField("areaMaxRange");
        }

        lua.PushString(skill.codename);
        lua.SetField("codename");

        lua.PushTable();

        for (unsigned i {}; i < skill.attackList.size(); ++i)
        {
            auto& attackPair = skill.attackList[i];
            auto& attack = attackPair.second;

            TealAssert(attack.get(), "Attack nullptr");

            lua.PushInteger(i + 1);
            lua.PushTable();

            {
                lua.PushString(AttackData::targetToString(attack->data.target));
                lua.SetField("target");

                DamageData* dmg {};
                StateData* state {};
                EffectData* effect {};

                switch (attackPair.first)
                {
                    case SkillData::AttackType::Damage:
                        dmg = static_cast<DamageData*>(attack.get());

                        lua.PushString(elementToString(dmg->damage.first));
                        lua.SetField("element");

                        lua.PushInteger(dmg->damage.second);
                        lua.SetField("damage");

                        lua.PushString("damage");
                        break;

                    case SkillData::AttackType::State:
                        state = static_cast<StateData*>(attack.get());

                        // TODO

                        lua.PushString("state");
                        break;

                    case SkillData::AttackType::Effect:
                        effect = static_cast<EffectData*>(attack.get());

                        // TODO

                        lua.PushString("effect");
                        break;

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

    return true;
}


void AISystem::Teal_MoveCharacter(unsigned x, unsigned y)
{
    TealAssert(m_isFightActive, "Not fighting");
    auto& move = m_currentFight.currentEntity->GetComponent<MoveComponent>();

    move.tile = { x, y };
    move.playerInitiated = true; // Actually it's not player initiated but it will makes sure the MovementSystem
                                 // won't try to recompute the path. It's a fight...

    m_currentFight.canResume = [this, x, y] ()
    {
        return m_currentFight.currentEntity->GetComponent<PositionComponent>().xy == AbsTile { x, y };
    };
}

void AISystem::Teal_TakeCover()
{
    TealAssert(m_isFightActive, "Not fighting");
    // todo: do strange calculations with scores and things
}

void AISystem::Teal_AttackCharacter(unsigned characterIndex, Nz::String skillCodename)
{
    TealAssert(m_isFightActive, "Not fighting");

    if (characterIndex >= m_currentFight.fighters.size())
    {
        NazaraError("AI: Character out of bounds !");
        return;
    }

    auto& fight = m_currentFight.currentEntity->GetComponent<FightComponent>();
    fight.target = m_currentFight.fighters[characterIndex];
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

unsigned AISystem::Teal_ChooseAttack(unsigned characterIndex) // todo: raycasting ?
{
    TealAssert(m_isFightActive, "Not fighting");

    if (!Teal_CanAttack(characterIndex))
        return 0;

    Ndk::EntityHandle opponent = m_currentFight.fighters[characterIndex];
    Ndk::EntityHandle me = m_currentFight.currentEntity;

    AbsTile opponentPos = opponent->GetComponent<PositionComponent>().xy;
    AbsTile myPos = me->GetComponent<PositionComponent>().xy;

    //...

    return 0;
}

bool AISystem::Teal_CanAttack(unsigned characterIndex) // todo: raycasting ?
{
    TealAssert(m_isFightActive, "Not fighting");

    if (characterIndex >= m_currentFight.fighters.size())
    {
        NazaraError("AI: Character out of bounds !");
        return false;
    }

    Ndk::EntityHandle opponent = m_currentFight.fighters[characterIndex];
    AbsTile opponentPos = opponent->GetComponent<PositionComponent>().xy;
    auto& opponentFight = opponent->GetComponent<FightComponent>();

    Ndk::EntityHandle me = m_currentFight.currentEntity;
    AbsTile myPos = me->GetComponent<PositionComponent>().xy;
    auto& myFight = me->GetComponent<FightComponent>();
    
    for (auto skillId : myFight.attacks)
    {
        const SkillData& skill = m_skills.getItem(skillId);

        if (myFight.actionPoints < skill.actionPoints || myFight.movementPoints < skill.movementPoints)
            continue;

        bool allyOnly { true };
        AttackData::Target attackTypeToAvoid { myFight.teamNumber == opponentFight.teamNumber ? AttackData::Target::Enemies : AttackData::Target::Allies };

        for (auto& attack : skill.attackList)
        {
            if (attack.second->data.target != attackTypeToAvoid)
            {
                allyOnly = false;
                break;
            }
        }

        if (allyOnly)
            continue;

        Nz::Vector2ui posDistance = { distance(myPos.x, opponentPos.x), distance(myPos.y, opponentPos.y) };

        if (posDistance.x < skill.minRange || posDistance.x > skill.maxRange ||
            posDistance.y < skill.minRange || posDistance.y > skill.maxRange)
            continue; // todo: check area min/max range & type, in case of "damage zone"

        if (skill.maxRange > 1 && !skill.viewThroughWalls)
        {
            NazaraError("Raycasting has not been implemented (yet)");
            continue; // todo: implement raycasting thing here
        }

        return true;
    }

    return false;
}

