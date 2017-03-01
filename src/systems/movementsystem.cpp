// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems/movementsystem.hpp"

MovementSystem::MovementSystem()
{
    Requires<PathComponent, PositionComponent, OrientationComponent>();
    SetUpdateRate(Def::MAXFPS);
}

void MovementSystem::OnUpdate(float elapsed)
{
    NazaraUnused(elapsed);

    for (auto& e : GetEntities())
    {
        auto& path = e->GetComponent<PathComponent>().path;
        auto& pos = e->GetComponent<PositionComponent>();

        if (path.empty())
            continue; // No path, no move.

        auto& orient = e->GetComponent<OrientationComponent>().dir;
        auto& dir = path.front();

        orient = DirToOrient(dir.first);
        auto xy = DirToXY(dir.first);

        int moveX { xy.x };
        int moveY { xy.y };

        bool walkMode = (path.size() == 1); // We almost finished, let's stop running

        if (walkMode)
        {
            moveX = (moveX == 2 || moveX == -2) ? moveX / 2 : moveX;
            moveY = (moveY == 2 || moveY == -2) ? moveY / 2 : moveY;
        }

        if (!pos.moving)
        {
            pos.moving = true;
            continue; // Continue so the animation system can animate.
        }

        pos.inX += moveX;
        pos.inY += moveY;

        if (pos.inX % Def::MAXPOSINTILE == 0) // Next horizontal tile reached
        {
            if (pos.inX > 0)
                pos.x += (pos.inX / Def::MAXPOSINTILE);
            else
                pos.x -= (-pos.inX / Def::MAXPOSINTILE);

            pos.inX = 0;
        }

        if (pos.inY % Def::MAXPOSINTILE == 0) // Next vertical tile reached
        {
            if (pos.inY > 0)
                pos.y += (pos.inY / Def::MAXPOSINTILE);
            else
                pos.y -= (-pos.inY / Def::MAXPOSINTILE);

            pos.inY = 0;
        }

        if (pos.inX == 0 && pos.inY == 0) // Next tile reached
        {
            if (!dir.second)
                path.pop(); // To get next tile
            else
                dir.second = false;
        }

        if (path.empty()) // Finished path
        {
            pos.moving = false; // Not moving anymore

            if (hasComponentsToChangeMap(e))
                if (e->GetComponent<MapPositionComponent>().canChangeMap)
                    changeMap(e);
        }

        if (isMapEntity(e))
            refreshGraphicsPos(e);
    }
}
