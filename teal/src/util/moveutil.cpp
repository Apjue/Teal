// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <algorithm>
#include "components/characters/pathcomponent.hpp"
#include "components/characters/positioncomponent.hpp"
#include "components/characters/orientationcomponent.hpp"
#include "components/shared/blocktilecomponent.hpp"
#include "components/characters/movecomponent.hpp"
#include "components/characters/inventorycomponent.hpp"
#include "components/shared/iconcomponent.hpp"
#include "components/util/logicentityidcomponent.hpp"
#include "components/util/graphicalentitiescomponent.hpp"
#include "def/gamedef.hpp"
#include "data/mapdata.hpp"
#include "containers/tilesetcore.hpp"
#include "util/maputil.hpp"
#include "util/gfxutil.hpp"
#include "util/entityutil.hpp"
#include "util/assert.hpp"
#include "util/moveutil.hpp"
#include "util/animutil.hpp"

void moveEntity(const Ndk::EntityHandle& e, bool fightMode)
{
    auto& pathComp = e->GetComponent<PathComponent>();
    auto& path = pathComp.path;

    if (path.empty())
        return; // No path, no move.

    auto& pos = e->GetComponent<PositionComponent>();
    bool even = isLineEven(pos.xy.y);
    auto& dir = path.front();
    auto   xy = DirToXY(dir, even);

    e->GetComponent<OrientationComponent>().orientation = DirToOrient(dir);

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

        path.erase(path.begin()); // To get next tile

        if (path.empty() && e->HasComponent<BlockTileComponent>())
            e->GetComponent<BlockTileComponent>().active = true; // Reset blocktile

        if (e->HasComponent<BlockTileComponent>() && e->GetComponent<BlockTileComponent>().active)
        {
            refreshOccupiedTiles();
            clearPatherCache();
        }

        if (!path.empty() && !fightMode && e->HasComponent<MoveComponent>())
            recomputePathIfObstacle(e);
    }

    // Update animation
    updateAnimation(e);

    // Update position
    if (isMapEntity(e))
        refreshGraphicsPos(e);

    if (e->HasComponent<GraphicalEntitiesComponent>())
        for (auto& gfxCharacter : e->GetComponent<GraphicalEntitiesComponent>().entities)
            refreshGraphicsPos(gfxCharacter);
}

void recomputePathIfObstacle(const Ndk::EntityHandle& e)
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

        auto& tile = currentMap->getTile(posX, posY);

        if (!tile.isWalkable())
        {
            auto& move = e->GetComponent<MoveComponent>();
            auto  dirs = directionsToPositions(path, pos.xy);

            move.tile = dirs.back();
            move.playerInitiated = false;
        }
    }
}

void getItemsFromGround(const Ndk::EntityHandle& e) /// todo: activate traps from ground or something. (fight) | todo 2: what is this function doing in this file ?
{
    TealAssert(isMapUtilityInitialized(), "Map Utility isn't initialized !");
    MapDataRef currentMap = getCurrentMap()->getCurrentMap();

    auto& mapEntities = currentMap->getGraphicalEntities();

    if (mapEntities.empty())
        return;

    auto& inv = e->GetComponent<InventoryComponent>();
    Ndk::EntityList killedEntities; // Ndk::Entity::Kill() is not immediate, needs a world refresh

    for (auto it = mapEntities.begin(); it != mapEntities.end();)
    {
        it = std::find_if(mapEntities.begin(), mapEntities.end(), [&e, &killedEntities] (const Ndk::EntityHandle& item)
        {
            if (!killedEntities.Has(item) && item.IsValid() && isValidGraphicalItemEntity(item))
                return item->GetComponent<PositionComponent>().xy == e->GetComponent<PositionComponent>().xy;

            return false;
        });

        if (it == mapEntities.end())
            break;

        TealAssert((*it)->GetComponent<LogicEntityIdComponent>().logicEntity.IsValid(), "Logic Entity isn't valid");
        inv.insert((*it)->GetComponent<LogicEntityIdComponent>().logicEntity);

        killedEntities.Insert(*it);
        (*it)->Kill(); // Nothing personal.
    }
}
