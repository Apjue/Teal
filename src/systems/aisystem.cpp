// Copyright (C) 2016 Samy Bensaid
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
#include "util/gameutil.hpp"
#include "def/gamedef.hpp"
#include "def/systemdef.hpp"
#include "data/elementdata.hpp"
#include "systems/aisystem.hpp"

AISystem::AISystem(const SkillStore& skills, const Nz::String& utilFilepath, const std::shared_ptr<micropather::MicroPather>& pather)
    : m_utilityLuaFile { utilFilepath }, m_skills(skills)
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

void AISystem::OnUpdate(float)
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

                AbsTile startPos = pos.xy;

                if (move.tile == startPos)
                {
                    move.tile = toVector(Def::StandStillPos);
                    continue;
                }

                AbsTile lastPos { move.tile };

                auto currentPath = directionsToPositions(path, startPos);

                if (!currentPath.empty() && lastPos == currentPath.back() && move.playerInitiated) // If user clicked to go to the same location, stop.
                {                                                                                  // Else, recompute path in case an object moved and blocks the path
                    move.tile = toVector(Def::StandStillPos);                                      // (See MovementSystem)
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

        /*if (e->HasComponent<FightComponent>() && e->HasComponent<LifeComponent>() && e != getMainCharacter()) // Compute fight
        {
            auto& fight = e->GetComponent<FightComponent>();
            auto& life = e->GetComponent<LifeComponent>();

            if (fight.isFighting && fight.myTurn) // Time to act !
            {
                TealAssert(m_isFightActive, "Not fighting");
                Nz::LuaInstance& lua = m_currentFight.ai;

                if (m_currentFight.clean)
                {
                    m_currentFight.currentEntity = e;
                    lua.SetTimeLimit(Def::DefaultFightTimeLimit); // todo: specific time limits ?

                    if (!prepareLuaAI(lua))
                    {
                        NazaraError("Failed to prepare Lua AI");

                        fight.myTurn = false;
                        // Reset lua instance
                        m_currentFight.clean = true;
                        continue;
                    }

                    // coroutine
                }

                // Lua things here
                // Do one AI per monster (type) and some "generals" AIs. AICore ?
                // Do AIs in Lua

                fight.myTurn = false;
            }
        }*/
    }
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
            //...
        }

        lua.SetField("objects");
    }


    lua.SetGlobal("teal_fight_data");
    lua.Execute("setmetatable(teal_fight_data.character, Character)");

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
    
}

void AISystem::Teal_TakeCover()
{
    TealAssert(m_isFightActive, "Not fighting");

}

void AISystem::Teal_AttackCharacter(unsigned characterIndex, Nz::String skillCodename)
{
    TealAssert(m_isFightActive, "Not fighting");

}

void AISystem::Teal_MoveAndAttackCharacter(unsigned characterIndex, Nz::String skillCodename)
{
    TealAssert(m_isFightActive, "Not fighting");

}

unsigned AISystem::Teal_ChooseTarget()
{
    TealAssert(m_isFightActive, "Not fighting");
    return 0; // temporary
}

unsigned AISystem::Teal_ChooseAttack(unsigned characterIndex)
{
    TealAssert(m_isFightActive, "Not fighting");
    return 0; // temporary
}

bool AISystem::Teal_CanAttack(unsigned characterIndex)
{
    TealAssert(m_isFightActive, "Not fighting");
    return false; // temporary
}

