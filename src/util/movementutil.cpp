// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <algorithm>
#include "components/common/pathcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/orientationcomponent.hpp"
#include "components/common/blocktilecomponent.hpp"
#include "components/common/movecomponent.hpp"
#include "components/common/inventorycomponent.hpp"
#include "components/common/iconcomponent.hpp"
#include "components/common/logicentityidcomponent.hpp"
#include "components/common/graphicalentitiescomponent.hpp"
#include "def/gamedef.hpp"
#include "data/mapdata.hpp"
#include "cache/tilesetcore.hpp"
#include "util/gfxutil.hpp"
#include "util/maputil.hpp"
#include "util/entityutil.hpp"
#include "util/assert.hpp"
#include "util/gameutil.hpp"
#include "util/movementutil.hpp"

void moveEntity(const Ndk::EntityHandle& e, bool allowMapInteractions)
{
    auto& pathComp = e->GetComponent<PathComponent>();
    auto& path = pathComp.path;

    if (path.empty())
        return; // No path, no move.

    auto& pos = e->GetComponent<PositionComponent>();
    bool even = isLineEven(pos.xy.y);
    auto& dir = path.front();
    auto   xy = DirToXY(dir, even);

    e->GetComponent<OrientationComponent>().dir = DirToOrient(dir);
    
    if (!pos.moving)
    {
        pos.moving = true;
        return; // Return so the animation system can animate.
    }

    pos.direction = dir;
    ++pos.advancement;
    
    if (pos.advancement >= Def::MaxPosInTile)
        pos.advancement = 0;

    if (pos.advancement == 0) // Next tile reached
    {
        const std::array<int, 8u>* mapDistanceX = (even ? &Def::MapDistanceEvenX : &Def::MapDistanceUnevenX);
        const std::array<int, 8u>* mapDistanceY = (even ? &Def::MapDistanceEvenY : &Def::MapDistanceUnevenY);

        pos.xy.x += (*mapDistanceX)[toUnderlyingType(DirToOrient(dir))];
        pos.xy.y += (*mapDistanceY)[toUnderlyingType(DirToOrient(dir))];

        std::swap(*(path.begin()), path.back());
        path.pop_back(); // To get next tile

        if (!path.empty() && allowMapInteractions && e->HasComponent<MoveComponent>())
            recomputeIfObstacle(e);
    }

    if (path.empty()) // Finished path
    {
        pos.moving = false; // Not moving anymore

        if (e == getMainCharacter() && allowMapInteractions)
        {
            TealAssert(hasComponentsToChangeMap(e) && e->HasComponent<InventoryComponent>(), "Main character doesn't have required components ?");

            getItemsFromGround(e);
            changeMap();
        }

        if (e->HasComponent<BlockTileComponent>() && e->GetComponent<BlockTileComponent>().blockTile)
        {
            refreshOccupiedTiles();
            clearPatherCache();
        }
    }

    if (isMapEntity(e))
        refreshGraphicsPos(e);

    if (e->HasComponent<GraphicalEntitiesComponent>())
        for (auto& gfxCharacter : e->GetComponent<GraphicalEntitiesComponent>().entities)
            refreshGraphicsPos(e, gfxCharacter);
}

void recomputeIfObstacle(const Ndk::EntityHandle& e)
{
    TealAssert(isMapUtilityInitialized(), "Map Utility isn't initialized !");

    auto& path = e->GetComponent<PathComponent>().path;
    auto& pos = e->GetComponent<PositionComponent>();
    MapDataRef currentMap = getCurrentMap()->getCurrentMap();

    for (unsigned i {}, posX { pos.xy.x }, posY { pos.xy.y }; i < path.size(); ++i)
    {
        auto moveXY = DirToXY(path[i], isLineEven(posY));

        posX += moveXY.x;
        posY += moveXY.y;

        auto& tile = currentMap->tile(posX, posY);

        if (!tile.isWalkable())
        {
            auto& move = e->GetComponent<MoveComponent>();
            auto  dirs = directionsToPositions(path, pos.xy);

            move.tile = dirs.back();
            move.playerInitiated = false;
        }
    }
}

void getItemsFromGround(const Ndk::EntityHandle& e)
{
    TealAssert(isMapUtilityInitialized(), "Map Utility isn't initialized !");
    MapDataRef currentMap = getCurrentMap()->getCurrentMap();

    auto& inv = e->GetComponent<InventoryComponent>();
    auto& mapEntities = currentMap->getEntities();
    auto itIndex = mapEntities.begin();

    for (;;)
    {
        if (mapEntities.empty())
            break;

        auto it = std::find_if(itIndex, mapEntities.end(),
                               [&e] (const Ndk::EntityHandle& item)
        {
            return item->HasComponent<LogicEntityIdComponent>() && item->GetComponent<LogicEntityIdComponent>().logicEntity.IsValid() && isMapEntity(item) &&
                   item->GetComponent<LogicEntityIdComponent>().logicEntity->HasComponent<PositionComponent>() &&
                   item->GetComponent<LogicEntityIdComponent>().logicEntity->GetComponent<PositionComponent>().xy == e->GetComponent<PositionComponent>().xy;
        });

        if (it == mapEntities.end())
            break;

        TealAssert(it->IsValid() && (*it)->IsValid(), "Item isn't valid");
        TealAssert((*it)->GetComponent<LogicEntityIdComponent>().logicEntity.IsValid() &&
            (*it)->GetComponent<LogicEntityIdComponent>().logicEntity->IsValid(), "Pointed Item isn't valid");

        if (!(*it)->GetComponent<LogicEntityIdComponent>().logicEntity->HasComponent<Items::ItemComponent>())
        {
            if (*it == *(mapEntities.end() - 1))
                break;

            itIndex = it + 1;
            continue;
        }

        (*it)->GetComponent<LogicEntityIdComponent>().logicEntity->RemoveComponent<PositionComponent>();
        inv.items.Insert((*it)->GetComponent<LogicEntityIdComponent>().logicEntity);
        (*it)->Kill(); // I'm sorry.

        if (itIndex > it)
            --itIndex;

        mapEntities.Remove(*it);
    }
}
