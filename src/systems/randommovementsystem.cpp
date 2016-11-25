// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems/randommovementsystem.hpp"

RandomMovementSystem::RandomMovementSystem() : m_uni(0, 7)
{
    Requires<PositionComponent, MoveToComponent, RandomMovementComponent>();
    SetUpdateRate(10.f);
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
        auto& mov = e->GetComponent<MoveToComponent>();
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
            {
                unsigned direction = m_uni(rng);
                Orientation orient = static_cast<Orientation>(direction);
                DiffTile xy = OrienToXY(orient);

                mov.diffX = xy.x;
                mov.diffY = xy.y;
            }
            else
            {
                std::vector<unsigned> failList;
                for (unsigned counter {}; counter < 8; ++counter)
                {
                    if (failList.size() == 8)
                        break;

                    auto map = m_map.lock();

                    unsigned direction = m_uni(rng);

                    for (unsigned iCounter {}; 
                         std::find(failList.begin(), failList.end(), direction) != failList.end() && iCounter < 8; 
                         ++iCounter)
                    { /// TODO Optimize this
                        direction = m_uni(rng);
                    }

                    Orientation orient = static_cast<Orientation>(direction);
                    DiffTile xy = OrienToXY(orient);

                    unsigned Xpos = pos.x + xy.x;
                    unsigned Ypos = pos.y + xy.y;
                    MapInstance::XYToArray(Xpos, Ypos);

                    unsigned tile = map->map->obs()[MapInstance::XYToIndex(Xpos, Ypos)];

                    if (tile == 0)
                    {
                        mov.diffX = xy.x;
                        mov.diffY = xy.y;

                        break;
                    }
                    else
                        failList.push_back(direction);
                }
            }
        }
    }
}
