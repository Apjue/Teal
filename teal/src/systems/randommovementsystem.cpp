// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <unordered_set>
#include <algorithm>
#include <iterator>
#include "global.hpp"
#include "util/maputil.hpp"
#include "util/aiutil.hpp"
#include "util/assert.hpp"
#include "util/util.hpp"
#include "components/characters/positioncomponent.hpp"
#include "components/characters/movecomponent.hpp"
#include "components/characters/randommovementcomponent.hpp"
#include "components/characters/fightcomponent.hpp"
#include "systems/randommovementsystem.hpp"
#include "def/systemdef.hpp"

RandomMovementSystem::RandomMovementSystem() : m_uni(0, Def::TileArraySize)
{
    Requires<PositionComponent, MoveComponent, RandomMovementComponent>();
    SetMaximumUpdateRate(Def::MaxSystemUPS); // Can be removed
    SetUpdateOrder(Def::RandomMovementSystemUpdateOrder);
}

RandomMovementSystem::RandomMovementSystem(const std::shared_ptr<micropather::MicroPather>& pather) : RandomMovementSystem()
{
    setPather(pather);
}

void RandomMovementSystem::setPather(const std::shared_ptr<micropather::MicroPather>& pather)
{
    m_pather = pather;
}

void RandomMovementSystem::OnUpdate(float elapsed)
{
    RandomNumber<std::mt19937> rng;

    for (auto& e : GetEntities())
    {
        if (e->HasComponent<FightComponent>() && e->GetComponent<FightComponent>().isFighting)
            continue;

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

        const MapInstance* map = getCurrentMap();
        TealAssert(map->getCurrentMap().IsValid(), "Map isn't valid !");
        TealAssert(m_pather, "Pather isn't valid !");

        if (goSomewhere && map)
        {
            std::set<AbsTile> nearTiles = getVisibleTiles(pos.xy, rd.range, true);
            std::set<AbsTile> maxDistanceTiles;
            std::set<AbsTile> priorityTiles; // Tiles with exact path size

            for (const AbsTile& tile : nearTiles) // some tiles of nearTiles may be at a >rd.range distance (e.g. if there's an obstacle during the path)
            {
                std::vector<DirectionFlags> path = computePath(pos.xy, tile, m_pather.get());

                if (path.size() <= rd.range && !path.empty())
                {
                    maxDistanceTiles.emplace(tile);

                    if (path.size() == rd.range)
                        priorityTiles.emplace(tile);
                }
            }

            if (maxDistanceTiles.empty())
                return;

            unsigned randomNumber = m_uni(rng);

            if (priorityTiles.size() > 2)
            {
                randomNumber %= priorityTiles.size();

                auto it = priorityTiles.begin();
                std::advance(it, randomNumber);

                mov.tile = *it;
            }

            else
            {
                randomNumber %= maxDistanceTiles.size();

                auto it = maxDistanceTiles.begin();
                std::advance(it, randomNumber);

                mov.tile = *it;
            }
        }
    }
}
