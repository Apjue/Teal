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

            AbsTile startPos {};
            AbsTile lastPos {};

            auto newPath = computePath(e, m_pather.get(), &startPos, &lastPos);

            if (newPath.empty())
                continue; // Cannot generate a path :(

            auto currentPath = directionsToPositions(path, startPos);

            move.diffX = 0;
            move.diffY = 0;

            if (!currentPath.empty() && lastPos == currentPath.back())
                continue;

            path = newPath;

            pos.inX = 0;
            pos.inY = 0;
        }
    }
}
