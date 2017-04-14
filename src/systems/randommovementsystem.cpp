// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems/randommovementsystem.hpp"

RandomMovementSystem::RandomMovementSystem() : m_uni(0, 7)
{
    Requires<PositionComponent, MoveComponent, RandomMovementComponent>();
    SetUpdateRate(10.f); // Can be removed
}

RandomMovementSystem::RandomMovementSystem(const std::weak_ptr<MapInstance>& map)
    : RandomMovementSystem()
{
    setMap(map);
}

void RandomMovementSystem::setMap(const std::weak_ptr<MapInstance>& map)
{
    m_map = map;
}

void RandomMovementSystem::OnUpdate(float elapsed)
{
    RandomNumber<std::mt19937> rng;

    for (auto& e : GetEntities())
    {
        auto& rd = e->GetComponent<RandomMovementComponent>();
        auto& mov = e->GetComponent<MoveComponent>();
        auto& pos = e->GetComponent<PositionComponent>();

        rd.currentInterval += elapsed;
        bool goSomewhere { false };

        while (rd.currentInterval > rd.movingInterval)
        {
            rd.currentInterval -= rd.movingInterval;
            goSomewhere = true;
        }

        if (goSomewhere)
        {
            if (m_map.expired())
                for (unsigned counter {}; counter < rd.nbTiles; ++counter)
                {
                    unsigned direction = m_uni(rng);
                    Orientation orient = static_cast<Orientation>(direction);
                    DiffTile xy = OrientToDiff(orient);

                    mov.diffX = mov.diffX + xy.x;
                    mov.diffY = mov.diffY + xy.y;
                }

            else
            {
                auto map = m_map.lock();
                NazaraAssert(map->getMap().IsValid(), "Map isn't valid !");

                for (unsigned counter {}; counter < rd.nbTiles; ++counter)
                {
                    unsigned x = pos.x;
                    unsigned y = pos.y;

                    XYToArray(x, y);

                    auto adjacentTiles = map->getMap()->adjacentTiles(x, y);

                    if (adjacentTiles.size() == 0)
                        break;

                    DiffTile xy;

                    if (adjacentTiles.size() > 1)
                    {
                        unsigned direction = m_uni(rng);

                        if (adjacentTiles.size() != 8)
                            direction %= adjacentTiles.size();

                        Orientation orient = static_cast<Orientation>(direction);
                        xy = OrientToDiff(orient);
                    }

                    else
                        xy = AbsPosToDiff({ pos.x, pos.y }, adjacentTiles.begin()->first);

                    unsigned newXpos = pos.x + xy.x;
                    unsigned newYpos = pos.y + xy.y;

                    XYToArray(newXpos, newYpos);

                    unsigned tile = map->getMap()->obs()[XYToIndex(newXpos, newYpos)];

                    if (tile == 0)
                    {
                        mov.diffX = mov.diffX + xy.x;
                        mov.diffY = mov.diffY + xy.y;

                        break;
                    }
                }
            }
        }
    }
}
