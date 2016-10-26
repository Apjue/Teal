// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems/randommovementsystem.hpp"

RandomMovementSystem::RandomMovementSystem() : m_uni(0, 7)
{
    Requires<PositionComponent, MoveToComponent, RandomMovementComponent>();
    SetUpdateRate(10.f);
}

void RandomMovementSystem::OnUpdate(float elapsed)
{
    RandomNumber<std::mt19937> rng;

    for (auto& e : GetEntities())
    {
        auto& rd = e->GetComponent<RandomMovementComponent>();
        auto& mov = e->GetComponent<MoveToComponent>();

        rd.currentInterval += elapsed;
        bool goSomewhere { false };

        while (rd.currentInterval > rd.movingInterval)
        {
            rd.currentInterval -= rd.movingInterval;
            goSomewhere = true;
        }

        if (goSomewhere)
        {
            unsigned direction = m_uni(rng);
            Orientation orient = static_cast<Orientation>(direction);
            DiffTile xy = OrienToXY(orient);

            mov.diffX = xy.x;
            mov.diffY = xy.y;
        }
    }
}
