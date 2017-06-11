// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems/movementsystem.hpp"

MovementSystem::MovementSystem()
{
    Requires<PathComponent, PositionComponent, OrientationComponent>();
    SetUpdateRate(Def::MAXFPS);
    SetUpdateOrder(2);
}

void MovementSystem::OnUpdate(float elapsed)
{
    NazaraUnused(elapsed);

    for (auto& e : GetEntities())
    {
        auto& path = e->GetComponent<PathComponent>().path;

        if (path.empty())
            continue; // No path, no move.

        moveEntity(e);
    }
}
