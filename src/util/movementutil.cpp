// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/movementutil.hpp"
#include "cache/tilesetcore.hpp"
#include "util/assert.hpp"

void moveEntity(const Ndk::EntityHandle& e)
{
    auto& pathComp = e->GetComponent<PathComponent>();
    auto& path = pathComp.path;
    auto& pos = e->GetComponent<PositionComponent>();

    if (path.empty())
        return; // No path, no move.

    bool  even = isLineEven(pos.xy.y);
    auto& dir = path.front();
    auto  xy = DirToXY(dir, even);

    e->GetComponent<OrientationComponent>().dir = DirToOrient(dir);
    
    if (!pos.moving)
    {
        pos.moving = true;
        return; // Return so the animation system can animate.
    }

    pos.direction = dir;
    ++pos.advancement;
    
    if (pos.advancement >= Def::MAXPOSINTILE)
        pos.advancement = 0;

    if (pos.advancement == 0) // Next tile reached
    {
        const std::array<int, 8u>* mapDistanceX = (even ? &Def::MAP_DISTANCE_EVEN_X : &Def::MAP_DISTANCE_UNEVEN_X);
        const std::array<int, 8u>* mapDistanceY = (even ? &Def::MAP_DISTANCE_EVEN_Y : &Def::MAP_DISTANCE_UNEVEN_Y);

        pos.xy.x += (*mapDistanceX)[toUnderlyingType(DirToOrient(dir))];
        pos.xy.y += (*mapDistanceY)[toUnderlyingType(DirToOrient(dir))];

        std::swap(*(path.begin()), path.back());
        path.pop_back(); // To get next tile

        if (!path.empty() && e->HasComponent<MapPositionComponent>() && e->HasComponent<MoveComponent>())
            recomputeIfObstacle(e);
    }

    if (path.empty()) // Finished path
    {
        pos.moving = false; // Not moving anymore

        if (e == getMainCharacter())
        {
            if (hasComponentsToChangeMap(e))
            {
                if (e->HasComponent<InventoryComponent>())
                    getItemsFromGround(e);

                changeMap();
            }
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
    auto& path = e->GetComponent<PathComponent>().path;
    auto& pos = e->GetComponent<PositionComponent>();

    auto& mapPos = e->GetComponent<MapPositionComponent>();
    MapDataRef currentMap = MapDataLibrary::Get(mapXYToString(mapPos.xy.x, mapPos.xy.y));

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
    auto& mapPos = e->GetComponent<MapPositionComponent>();
    MapDataRef currentMap = MapDataLibrary::Get(mapXYToString(mapPos.xy.x, mapPos.xy.y));

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
