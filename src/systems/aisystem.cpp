// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems/aisystem.hpp"

AISystem::AISystem()
{
    Requires<PathComponent, PositionComponent, MoveComponent>();
}

AISystem::AISystem(const std::shared_ptr<micropather::MicroPather>& pather)
    : AISystem()
{
    setPather(pather);
}

void AISystem::reset()
{
    NazaraAssert(m_pather, "Pather is null, cannot reset it !");
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

        if (move.diffX != 0 || move.diffY != 0) // This entity wants to move
        {
            auto& pos = e->GetComponent<PositionComponent>();
            auto& path = e->GetComponent<PathComponent>().path;

            AbsTile startPos { pos.x, pos.y };
            AbsTile lastPos { static_cast<int>(pos.x) + move.diffX,
                              static_cast<int>(pos.x) + move.diffX };

            auto currentPath = directionsToPositions(path, startPos);

            if (!currentPath.empty() && lastPos == currentPath.back())
            {
                move.diffX = 0;
                move.diffY = 0;

                continue;
            }

            auto newPath = computePath(e, m_pather.get());

            if (newPath.empty())
                continue; // Cannot generate a path :(

            path = newPath;

            pos.inX = 0;
            pos.inY = 0;

            move.diffX = 0;
            move.diffY = 0;
        }

        /*
        Todo: create another component & system for fight
        With a queue, like path queue
        queue will have things like:
         - Move Up|Left
         - Attack with Legendary Sword
         - Move Right
        Path will be mixed with attacks, so it is necessary
        to have a new component/system to keep the order
        */

        if (e->HasComponent<FightComponent>())
        {
            auto& fight = e->GetComponent<FightComponent>();

            if (fight.isFighting && fight.myTurn) // Time to act !
            {
                // Oh no, I haven't do this part yet
                // I can't kill the monsters :(

                fight.myTurn = false;
            }
        }
    }
}
