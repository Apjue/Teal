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
            auto& pathComp = e->GetComponent<PathComponent>();
            auto& path = pathComp.path;

            AbsTile startPos { pos.x, pos.y };
            AbsTile lastPos { itou(utoi(pos.x) + move.diffX),
                              itou(utoi(pos.y) + move.diffY) };

            if (isPositionValid(startPos))
                pathComp.oldWantedPos = { Def::LMAPX + 2, Def::LMAPY + 2 };

            if (pathComp.wantedPos == pathComp.oldWantedPos) // Stop spam-clicking
                continue;

            pathComp.oldWantedPos = pathComp.wantedPos;
            pathComp.wantedPos = { Def::LMAPX + 1, Def::LMAPY + 1 };

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
            pathComp.totalSize = static_cast<unsigned>(path.size());

            pos.inX = 0;
            pos.inY = 0;

            move.diffX = 0;
            move.diffY = 0;

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
        */

        if (e->HasComponent<FightComponent>() && e->HasComponent<LifeComponent>())
        {
            auto& fight = e->GetComponent<FightComponent>();
            auto& life = e->GetComponent<LifeComponent>();

            if (fight.isFighting && fight.myTurn) // Time to act !
            {
                // Oh no, I haven't do this part yet
                // I can't kill the monsters :(

                fight.myTurn = false;

                // Generate automatically attacks and co here
                // Unless it's the main player. To verify: getMainCharacter()

                // Do 3 AIs (per type of monster + 1 general AI): Neutral, Offensive, Defensive
                // Neutral choose between offensive & defensive
                // example: (health < maxHealth / 4) ? defensive : offensive;

                // Do "level" of AI: Stupid, Average, Smart, etc.
            }
        }
    }
}
