// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Nazara/Lua/LuaInstance.hpp>
#include "components/common/pathcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/movecomponent.hpp"
#include "components/common/fightcomponent.hpp"
#include "components/common/lifecomponent.hpp"
#include "util/aiutil.hpp"
#include "util/nzstlcompatibility.hpp"
#include "util/maputil.hpp"
#include "util/gameutil.hpp"
#include "def/gamedef.hpp"
#include "def/systemdef.hpp"
#include "systems/aisystem.hpp"

AISystem::AISystem()
{
    Requires<PathComponent, PositionComponent, MoveComponent>();
    SetUpdateOrder(Def::AISystemUpdateOrder);
}

AISystem::AISystem(const std::shared_ptr<micropather::MicroPather>& pather)
    : AISystem()
{
    setPather(pather);
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
    NazaraUnused(elapsed);

    for (auto& e : GetEntities())
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
                move.tile = toVector(Def::StandStillPos);                                          // (See MovementSystem)
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

        if (e->HasComponent<FightComponent>() && e->HasComponent<LifeComponent>() && e != getMainCharacter())
        {
            auto& fight = e->GetComponent<FightComponent>();
            auto& life = e->GetComponent<LifeComponent>();

            if (fight.isFighting && fight.myTurn) // Time to act !
            {
                Nz::LuaInstance lua;

                if (!prepareLuaAI(lua, e, m_currentFight))
                {
                    NazaraError("Failed to prepare Lua AI");

                    fight.myTurn = false;
                    continue;
                }

                // Lua things here
                // Do one AI per monster (type) and some "generals" AIs. AICore ?
                // Do AIs in Lua

                fight.myTurn = false;
            }
        }
    }
}

bool prepareLuaAI(Nz::LuaInstance& lua, const Ndk::EntityHandle& character, const detail::FightData& fight)
{
    return false;
}
