// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <unordered_set>
#include <algorithm>
#include "global.hpp"
#include "util/assert.hpp"
#include "util/util.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/movecomponent.hpp"
#include "components/common/randommovementcomponent.hpp"
#include "components/common/fightcomponent.hpp"
#include "systems/randommovementsystem.hpp"
#include "def/systemdef.hpp"

RandomMovementSystem::RandomMovementSystem() : m_uni(0, 7)
{
    Requires<PositionComponent, MoveComponent, RandomMovementComponent>();
    SetFixedUpdateRate(10.f); // Can be removed
    SetUpdateOrder(Def::RandomMovementSystemUpdateOrder);
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

        goSomewhere = e->HasComponent<FightComponent>() && e->GetComponent<FightComponent>().isFighting ? false : goSomewhere;

        if (goSomewhere && !m_map.expired())
        {
            auto map = m_map.lock();
            TealAssert(map->getCurrentMap().IsValid(), "Map isn't valid !");

            for (unsigned counter {}, x {}, y {}; counter < rd.nbTiles; ++counter)
            { /// \todo Redo this. Does not work as intended
                AbsTile wantedPos = (mov.tile == toVector(Def::StandStillPos) ? pos.xy : mov.tile);

                x = wantedPos.x;
                y = wantedPos.y;
                
                auto adjacentTiles = map->getCurrentMap()->adjacentTiles(x, y);

                if (adjacentTiles.empty())
                    break;

                DiffTile xy;

                if (adjacentTiles.size() > 1)
                {
                    unsigned direction = m_uni(rng);

                    if (adjacentTiles.size() != 8)
                        direction %= adjacentTiles.size();

                    Orientation orient = static_cast<Orientation>(direction);
                    xy = OrientToDiff(orient, isLineEven(y));
                }
                
                else
                    xy = AbsPosToDiff(wantedPos, adjacentTiles.begin()->first);

                AbsTile newPos { wantedPos.x + xy.x, wantedPos.y + xy.y };

                unsigned newXpos = newPos.x;
                unsigned newYpos = newPos.y;

                if (map->getCurrentMap()->tile(XYToIndex(newXpos, newYpos)).isWalkable())
                {
                    wantedPos.x += xy.x;
                    wantedPos.y += xy.y;

                    mov.tile = wantedPos;
                    mov.playerInitiated = false;

                    break;
                }
            }
        }
    }
}
