// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <memory>
#include "components/common/positioncomponent.hpp"
#include "components/common/mappositioncomponent.hpp"
#include "components/common/orientationcomponent.hpp"
#include "cache/tilesetcore.hpp"
#include "util/maputil.hpp"

namespace
{

std::weak_ptr<MapInstance> m_currentMap;
std::weak_ptr<micropather::MicroPather> m_pather;
Ndk::EntityHandle m_mainChar;

}

std::pair<bool, DirectionFlags> canChangeMap(const Ndk::EntityHandle& p)
{
    TealAssert(isMapUtilityInitialized(), "Map Utility hasn't been initialized !");
    TealAssert(hasComponentsToChangeMap(p), "Entity doesn't have the right components to change map !");

    auto& mapPos = p->GetComponent<MapPositionComponent>();
    auto& pos = p->GetComponent<PositionComponent>();

    // Where is the entity in the map ? Right, left, down, or up ?
    DirectionFlags entExt = 0; // Entity Extremity

    if (pos.xy.x == 0u && MapDataLibrary::Has(mapXYToString(mapPos.xy.x - 1, mapPos.xy.y))) // Left
        entExt = Dir::Left;

    else if (pos.xy.x == Def::MapX && MapDataLibrary::Has(mapXYToString(mapPos.xy.x + 1, mapPos.xy.y))) // Right
        entExt = Dir::Right;

    else if (pos.xy.y == 0u && MapDataLibrary::Has(mapXYToString(mapPos.xy.x, mapPos.xy.y + 1))) // Up
        entExt = Dir::Up;

    else if (pos.xy.y == Def::ArrayMapY && MapDataLibrary::Has(mapXYToString(mapPos.xy.x, mapPos.xy.y - 1))) // Down
        entExt = Dir::Down;

    if (!entExt)
        return std::make_pair(false, entExt); // Entity isn't even at an extremity

    // Okay, now, let's check if the position where the entity
    // will move to is valid (no obstacle)

    MapDataRef map; // Map the entity will move to
    unsigned x {}, y {}; // New position of the entity after changing map

    if (entExt & Dir::Left)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.xy.x - 1, mapPos.xy.y));

        x = Def::MapX;
        y = pos.xy.y;
    }

    else if (entExt & Dir::Right)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.xy.x + 1, mapPos.xy.y));

        x = 0u;
        y = pos.xy.y;
    }

    else if (entExt & Dir::Up)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.xy.x, mapPos.xy.y + 1));

        x = pos.xy.x;
        y = Def::MapY;
    }

    else if (entExt & Dir::Down)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.xy.x, mapPos.xy.y - 1));

        x = pos.xy.x;
        y = 1u;
    }

    else
    {
        NazaraError("Bad direction value");
        return std::make_pair(false, entExt);
    }

    TealAssert(map, "new map null !");
    
    if (!map->tile(XYToIndex(x, y)).isWalkable())
        return std::make_pair(false, entExt); // It's an obstacle.

    return std::make_pair(true, entExt);
}

bool changeMap()
{
    TealAssert(isMapUtilityInitialized(), "Map Utility hasn't been initialized !");
    auto canChange = canChangeMap(m_mainChar);

    if (!canChange.first)
        return false;

    auto& mapPos = m_mainChar->GetComponent<MapPositionComponent>();
    auto& pos = m_mainChar->GetComponent<PositionComponent>();

    MapDataRef newMap; // Map the entity will move to
    unsigned x {}, y {}; // New position of the entity after changing map
    unsigned mapX {}, mapY {}; // Position of the new map
    Orientation newOrient { Orientation::Down };

    if (canChange.second & Dir::Left)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.xy.x - 1, mapPos.xy.y));

        x = Def::MapX;
        y = pos.xy.y;

        mapX = mapPos.xy.x - 1;
        mapY = mapPos.xy.y;

        newOrient = Orientation::Left;

    }

    else if (canChange.second & Dir::Right)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.xy.x + 1, mapPos.xy.y));

        x = 0u;
        y = pos.xy.y;

        mapX = mapPos.xy.x + 1;
        mapY = mapPos.xy.y;

        newOrient = Orientation::Right;

    }

    else if (canChange.second & Dir::Up)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.xy.x, mapPos.xy.y + 1));

        x = pos.xy.x;
        y = Def::MapY;

        mapX = mapPos.xy.x;
        mapY = mapPos.xy.y + 1;

        newOrient = Orientation::Up;

    }

    else if (canChange.second & Dir::Down)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.xy.x, mapPos.xy.y - 1));

        x = pos.xy.x;
        y = 1u;

        mapX = mapPos.xy.x;
        mapY = mapPos.xy.y - 1;

        newOrient = Orientation::Down;
    }

    TealAssert(newMap, "new map null !");
    auto currentMapLock = m_currentMap.lock();

    deactivateMapEntities(currentMapLock->getMap());
    currentMapLock->setMap(newMap);
    activateMapEntities(currentMapLock->getMap());

    currentMapLock->update();
    clearPatherCache();

    pos.xy = { x, y };

    mapPos.xy.x = mapX;
    mapPos.xy.y = mapY;

    if (m_mainChar->HasComponent<OrientationComponent>())
        m_mainChar->GetComponent<OrientationComponent>().dir = newOrient;

    return true;
}

void initMapUtility(const std::weak_ptr<MapInstance>& currentMap,
                    const std::weak_ptr<micropather::MicroPather>& pather,
                    const Ndk::EntityHandle& mainCharacter)
{
    m_currentMap = currentMap;
    m_pather = pather;
    m_mainChar = mainCharacter;
}

bool isMapUtilityInitialized()
{
    return !m_currentMap.expired() && !m_pather.expired() && m_mainChar.IsValid();
}

Ndk::EntityHandle getMainCharacter()
{
    TealAssert(isMapUtilityInitialized(), "Map Utility hasn't been initialized !");
    return m_mainChar;
}

std::queue<AbsTile> directionsToPositions(PathComponent::PathPool directions, AbsTile start)
{
    std::queue<AbsTile> positions;

    while (!directions.empty())
    {
        auto& dir = directions.front();
        auto xy = DirToXY(dir, isLineEven(start.y));

        start.x += xy.x;
        start.y += xy.y;

        positions.push(start);

        std::swap(*(directions.begin()), directions.back());
        directions.pop_back();
    }

    return positions;
}

void refreshOccupiedTiles()
{
    TealAssert(isMapUtilityInitialized(), "Map Utility hasn't been initialized !");
    m_currentMap.lock()->getMap()->updateOccupiedTiles();
}

void clearPatherCache()
{
    TealAssert(isMapUtilityInitialized(), "Map Utility hasn't been initialized !");
    m_pather.lock()->Reset();
}
