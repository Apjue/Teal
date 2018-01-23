// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components/NodeComponent.hpp>
#include <memory>
#include "components/common/positioncomponent.hpp"
#include "components/common/orientationcomponent.hpp"
#include "components/common/renderablesstoragecomponent.hpp"
#include "cache/tilesetcore.hpp"
#include "util/animutil.hpp"
#include "util/gfxutil.hpp"
#include "util/util.hpp"
#include "util/maputil.hpp"

namespace
{

MapInstance* m_currentMap {};
micropather::MicroPather* m_pather {};
Ndk::EntityHandle m_mainCharacter {};

}

std::pair<bool, DirectionFlags> canChangeMap(const Ndk::EntityHandle& p)
{
    TealAssert(isMapUtilityInitialized(), "Map Utility hasn't been initialized !");
    TealAssert(hasComponentsToChangeMap(p), "Entity doesn't have the right components to change map !");

    Nz::Vector2i mapPos = m_currentMap->getCurrentMap()->getPosition();
    auto& pos = p->GetComponent<PositionComponent>();

    TealAssert(isPositionValid(pos.xy), "Position isn't valid");

    // Where is the entity in the map ? Right, left, down, or up ?
    DirectionFlags entExt = 0; // Entity Extremity

    if (pos.xy.x == 0u && isLineEven(pos.xy.y) && MapDataLibrary::Has(mapXYToString(mapPos.x - 1, mapPos.y))) // Left
        entExt = Dir::Left;

    else if (pos.xy.x == Def::MapX && MapDataLibrary::Has(mapXYToString(mapPos.x + 1, mapPos.y))) // Right
        entExt = Dir::Right;

    else if (pos.xy.y == 0u && MapDataLibrary::Has(mapXYToString(mapPos.x, mapPos.y + 1))) // Up
        entExt = Dir::Up;

    else if (pos.xy.y == Def::ArrayMapY && MapDataLibrary::Has(mapXYToString(mapPos.x, mapPos.y - 1))) // Down
        entExt = Dir::Down;

    if (!entExt)
        return std::make_pair(false, entExt); // Entity isn't even at an extremity

    // Okay, now, let's check if the position where the entity
    // will move to is valid (no obstacle)

    MapDataRef map; // Map the entity will move to
    unsigned x {}, y {}; // New position of the entity after changing map

    if (entExt & Dir::Left)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.x - 1, mapPos.y));

        x = Def::MapX;
        y = pos.xy.y;
    }

    else if (entExt & Dir::Right)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.x + 1, mapPos.y));

        x = 0u;
        y = pos.xy.y;
    }

    else if (entExt & Dir::Up)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y + 1));

        x = pos.xy.x;
        y = Def::MapY;
    }

    else if (entExt & Dir::Down)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y - 1));

        x = pos.xy.x;
        y = 1u;
    }

    else
    {
        NazaraError("Bad direction value");
        return std::make_pair(false, entExt);
    }

    TealAssert(map, "new map null !");
    
    if (!map->getTile(XYToIndex(x, y)).isWalkable())
        return std::make_pair(false, entExt); // It's an obstacle.

    return std::make_pair(true, entExt);
}

bool changeMap()
{
    TealAssert(isMapUtilityInitialized(), "Map Utility hasn't been initialized !");
    auto canChange = canChangeMap(m_mainCharacter);

    if (!canChange.first)
        return false;

    auto& mapPos = m_currentMap->getCurrentMap()->getPosition();
    auto& pos = m_mainCharacter->GetComponent<PositionComponent>();

    MapDataRef newMap; // Map the entity will move to
    unsigned x {}, y {}; // New position of the entity after changing map
    Orientation newOrient { Orientation::Down };

    if (canChange.second & Dir::Left)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x - 1, mapPos.y));

        x = Def::MapX;
        y = pos.xy.y;

        newOrient = Orientation::Left;
    }

    else if (canChange.second & Dir::Right)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x + 1, mapPos.y));

        x = 0u;
        y = pos.xy.y;

        newOrient = Orientation::Right;
    }

    else if (canChange.second & Dir::Up)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y + 1));

        x = pos.xy.x;
        y = Def::MapY;

        newOrient = Orientation::Up;
    }

    else if (canChange.second & Dir::Down)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y - 1));

        x = pos.xy.x;
        y = 1u;

        newOrient = Orientation::Down;
    }

    TealAssert(newMap, "new map null !");

    deactivateMapEntities(m_currentMap->getCurrentMap());
    m_currentMap->setMap(m_currentMap->getCurrentMapIndex(), newMap);
    
    for (auto& entity : m_currentMap->getCurrentMap()->getEntities())
        if (hasComponentsToAnimate(entity))
            updateAnimation(entity);
    
    activateMapEntities(m_currentMap->getCurrentMap());

    m_currentMap->update();
    clearPatherCache();

    pos.xy = { x, y };
    m_mainCharacter->GetComponent<OrientationComponent>().dir = newOrient;
    refreshGraphicsPos(m_mainCharacter);

    return true;
}

extern void initializeMapUtility(MapInstance* currentMap, micropather::MicroPather* pather, const Ndk::EntityHandle& mainCharacter)
{
    m_currentMap = currentMap;
    m_pather = pather;
    m_mainCharacter = mainCharacter;
}

bool isMapUtilityInitialized()
{
    return m_currentMap && m_currentMap->getCurrentMap().IsValid() && m_pather && m_mainCharacter.IsValid();
}

Ndk::EntityList mapEntitiesHoveredByCursor(const Nz::Vector2ui& cursor)
{
    TealAssert(isMapUtilityInitialized(), "Map Utility hasn't been initialized !");

    const MapDataRef& map = getCurrentMap()->getCurrentMap();
    Ndk::EntityList entities;

    for (const Ndk::EntityHandle& entity : map->getEntities())
    {
        Nz::Vector3f nodePos = entity->GetComponent<Ndk::NodeComponent>().GetPosition();
        auto& renderables = entity->GetComponent<RenderablesStorageComponent>();

        for (auto& sprite : renderables.sprites)
        {
            auto& spriteSize = sprite->GetSize();
            Nz::Rectf box { nodePos.x, nodePos.y, spriteSize.x, spriteSize.y };

            if (box.Contains(float(cursor.x), float(cursor.y)))
                entities.Insert(entity);
        }
    }

    return entities;
}

void refreshOccupiedTiles()
{
    TealAssert(isMapUtilityInitialized(), "Map Utility hasn't been initialized !");
    m_currentMap->getCurrentMap()->updateOccupiedTiles();
}

void clearPatherCache()
{
    TealAssert(isMapUtilityInitialized(), "Map Utility hasn't been initialized !");
    m_pather->Reset();
}

const MapInstance* getCurrentMap()
{
    return m_currentMap;
}

const micropather::MicroPather* getPather()
{
    return m_pather;
}

std::vector<AbsTile> directionsToPositions(PathComponent::PathPool directions, AbsTile start)
{
    std::vector<AbsTile> positions;

    while (!directions.empty())
    {
        auto& dir = directions.front();
        DiffTile xy = DirToXY(dir, isLineEven(start.y));

        TealAssert(xy.x >= 0 || int(start.x) >= -xy.x, "Imminent underflow");
        TealAssert(xy.y >= 0 || int(start.y) >= -xy.y, "Imminent underflow");

        start.x += xy.x;
        start.y += xy.y;

        positions.push_back(start);
        directions.erase(directions.begin());
    }

    return positions;
}

std::vector<AbsTile> getVisibleTiles(AbsTile pos, unsigned range, bool viewThroughObstacles)
{
    throw std::runtime_error { "getVisibleTiles not implemented (yet)" };
}


Vector2uPair getTileCornerSegment(Orientation corner, unsigned x, unsigned y)
{
    Nz::Rectui aabb = getTileAABB(x, y);

    switch (corner)
    {
        case Orientation::Down:
            return Vector2uPair { Nz::Vector2ui { aabb.x, aabb.y + aabb.height },
                                  Nz::Vector2ui { aabb.x + aabb.width, aabb.y + aabb.height } };

        case Orientation::DownRight:
            return Vector2uPair { Nz::Vector2ui { aabb.x + aabb.width / 2, aabb.y + aabb.height },
                                  Nz::Vector2ui { aabb.x + aabb.width, aabb.y + aabb.height / 2 } };

        case Orientation::Right:
            return Vector2uPair { Nz::Vector2ui { aabb.x + aabb.width, aabb.y + aabb.height },
                                  Nz::Vector2ui { aabb.x + aabb.width, aabb.y} };

        case Orientation::UpRight:
            return Vector2uPair { Nz::Vector2ui { aabb.x + aabb.width, aabb.y + aabb.height / 2 },
                                  Nz::Vector2ui { aabb.x + aabb.width / 2, aabb.y } };

        case Orientation::Up:
            return Vector2uPair { Nz::Vector2ui { aabb.x + aabb.width, aabb.y },
                                  Nz::Vector2ui { aabb.x, aabb.y } };

        case Orientation::UpLeft:
            return Vector2uPair { Nz::Vector2ui { aabb.x + aabb.width / 2, aabb.y },
                                  Nz::Vector2ui { aabb.x, aabb.y + aabb.height / 2 } };

        case Orientation::Left:
            return Vector2uPair { Nz::Vector2ui { aabb.x, aabb.y },
                                  Nz::Vector2ui { aabb.x, aabb.y + aabb.height } };

        case Orientation::DownLeft:
            return Vector2uPair { Nz::Vector2ui { aabb.x, aabb.y + aabb.height / 2 },
                                  Nz::Vector2ui { aabb.x + aabb.width / 2, aabb.y + aabb.height } };
    }

    throw std::runtime_error { "Corner not initialized properly" };
}
