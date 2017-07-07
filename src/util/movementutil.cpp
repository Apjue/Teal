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

    auto& orient = e->GetComponent<OrientationComponent>().dir;
    auto& dir = path.front();

    orient = DirToOrient(dir.first);
    auto xy = DirToXY(dir.first);

    if (pathComp.totalSize == 1) // Walk mode if path is short
    {
        xy.x = (xy.x == 2 || xy.x == -2) ? xy.x / 2 : xy.x;
        xy.y = (xy.y == 2 || xy.y == -2) ? xy.y / 2 : xy.y;
    }

    if (!pos.moving)
    {
        pos.moving = true;
        return; // Return so the animation system can animate.
    }

    pos.inXY += xy;
    
    bool resetPosInX { true };
    
    if (pos.inXY.x > 0 && pos.inXY.x >= Def::MAXPOSINTILE)
        pos.xy.x += (pos.inXY.x / Def::MAXPOSINTILE);

    else if (-pos.inXY.x >= Def::MAXPOSINTILE)
        pos.xy.x -= (-pos.inXY.x / Def::MAXPOSINTILE);

    else
        resetPosInX = false;

    pos.inXY.x = resetPosInX ? 0 : pos.inXY.x;


    bool resetPosInY { true };

    if (pos.inXY.y > 0 && pos.inXY.y >= Def::MAXPOSINTILE)
        pos.xy.y += (pos.inXY.y / Def::MAXPOSINTILE);

    else if (-pos.inXY.y >= Def::MAXPOSINTILE)
        pos.xy.y -= (-pos.inXY.y / Def::MAXPOSINTILE);

    else
        resetPosInY = false;

    pos.inXY.y = resetPosInY ? 0 : pos.inXY.y;

    if (pos.inXY == pos.inXY.Zero()) // Next tile reached
    {
        if (!dir.second)
        {
            path.erase(path.begin()); // To get next tile

            if (!path.empty() && e->HasComponent<MapPositionComponent>() && e->HasComponent<MoveComponent>())
            {
                auto& mapPos = e->GetComponent<MapPositionComponent>();
                MapDataRef currentMap = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y));

                for (unsigned i {}, x {}, y {}, posX { pos.xy.x }, posY { pos.xy.y }; i < path.size(); ++i)
                {
                    auto moveXY = DirToXY(path[i].first);

                    x = posX + moveXY.x;
                    y = posY + moveXY.y;

                    XYToArray(x, y);

                    posX += moveXY.x;
                    posY += moveXY.y;

                    auto& tile = currentMap->tile(x, y);

                    if (tile.obstacle != 0 || tile.occupied)
                    {
                        auto& move = e->GetComponent<MoveComponent>();
                        auto dirs = directionsToPositions(path, pos.xy);

                        move.tile = dirs.back();
                        move.playerInitiated = false;
                    }
                }
            }
        }
        else
            dir.second = false;
    }

    if (path.empty()) // Finished path
    {
        pos.moving = false; // Not moving anymore
        pathComp.totalSize = 0u;

        if (e == getMainCharacter())
        {
            if (hasComponentsToChangeMap(e))
            {
                if (e->HasComponent<InventoryComponent>())
                {
                    auto& mapPos = e->GetComponent<MapPositionComponent>();
                    MapDataRef currentMap = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y));

                    auto& inv = e->GetComponent<InventoryComponent>();

                    for (;;)
                    {
                        auto it = std::find_if(currentMap->getEntities().begin(), currentMap->getEntities().end(),
                                               [&e] (const Ndk::EntityHandle& item)
                        { return isItemEntity(item) && item->HasComponent<PositionComponent>() &&
                                 item->GetComponent<PositionComponent>().xy == e->GetComponent<PositionComponent>().xy; });

                        if (it == currentMap->getEntities().end())
                            break;

                        TealAssert(it->IsValid() && (*it)->IsValid(), "Item isn't valid");

                        inv.add(*it);
                        currentMap->getEntities().Remove(*it);
                    }
                }

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
}
