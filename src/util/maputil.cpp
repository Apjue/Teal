// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/maputil.hpp"

namespace
{

std::weak_ptr<MapInstance> m_currentMap;
std::weak_ptr<micropather::MicroPather> m_pather;
Ndk::EntityHandle m_mainChar;

}

std::pair<bool, DirectionFlags> canChangeMap(const Ndk::EntityHandle& p)
{
    NazaraAssert(isMapUtilityInited(), "Map Utility hasn't been initialized !");
    NazaraAssert(hasComponentsToChangeMap(p), "Entity doesn't have the right components to change map !");

    auto& mapPos = p->GetComponent<MapPositionComponent>();
    auto& pos = p->GetComponent<PositionComponent>();

    // Where is the entity in the map ? Right, left, down, or up ?
    DirectionFlags entExt = 0; // Entity Extremity

    if (pos.x == 0u && MapDataLibrary::Has(mapXYToString(mapPos.x - 1, mapPos.y))) // Left
        entExt = Dir::Left;

    else if (pos.x == Def::LMAPX && MapDataLibrary::Has(mapXYToString(mapPos.x + 1, mapPos.y))) // Right
        entExt = Dir::Right;

    else if (pos.y == 0u && MapDataLibrary::Has(mapXYToString(mapPos.x, mapPos.y - 1))) // Up
        entExt = Dir::Up;

    else if (pos.y == Def::LMAPY && MapDataLibrary::Has(mapXYToString(mapPos.x, mapPos.y + 1))) // Down
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

        x = Def::LMAPX;
        y = pos.y;
    }

    else if (entExt & Dir::Right)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.x + 1, mapPos.y));

        x = 0u;
        y = pos.y;
    }

    else if (entExt & Dir::Up)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y - 1));

        x = pos.x;
        y = Def::LMAPY;
    }

    else if (entExt & Dir::Down)
    {
        map = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y + 1));

        x = pos.x;
        y = 0u;
    }

    else
    {
        NazaraError("Bad direction value");

        return std::make_pair(false, entExt);
    }

    NazaraAssert(map, "new map null !");

    MapInstance::XYToArray(x, y);

    if (map->obs()[x + y * Def::MAPX] != 0)
        return std::make_pair(false, entExt); // It's an obstacle.

    return std::make_pair(true, entExt);
}

bool changeMap(const Ndk::EntityHandle& p)
{
    NazaraAssert(p == m_mainChar, "changeMap must only be used with the main character (provided by initMapUtility)");
    auto canChange = canChangeMap(p);

    if (!canChange.first)
        return false;

    auto& mapPos = p->GetComponent<MapPositionComponent>();
    auto& pos = p->GetComponent<PositionComponent>();

    MapDataRef newMap; // Map the entity will move to
    unsigned x {}, y {}; // New position of the entity after changing map
    unsigned mapX {}, mapY {}; // Position of the new map
    Orientation newOrient { Orientation::Down };

    if (canChange.second & Dir::Left)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x - 1, mapPos.y));

        x = Def::LMAPX;
        y = pos.y;

        mapX = mapPos.x - 1;
        mapY = mapPos.y;

        newOrient = Orientation::Left;

    }

    else if (canChange.second & Dir::Right)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x + 1, mapPos.y));

        x = 0u;
        y = pos.y;

        mapX = mapPos.x + 1;
        mapY = mapPos.y;

        newOrient = Orientation::Right;

    }

    else if (canChange.second & Dir::Up)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y - 1));

        x = pos.x;
        y = Def::LMAPY;

        mapX = mapPos.x;
        mapY = mapPos.y - 1;

        newOrient = Orientation::Up;

    }

    else if (canChange.second & Dir::Down)
    {
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y + 1));

        x = pos.x;
        y = 0u;

        mapX = mapPos.x;
        mapY = mapPos.y + 1;

        newOrient = Orientation::Down;
    }

    NazaraAssert(newMap, "new map null !");

    auto currentMapLock = m_currentMap.lock();
    auto patherLock = m_pather.lock();

    deactivateMapEntities(currentMapLock->map);

    currentMapLock->map = newMap;
    patherLock->Reset(); // Map changed, need to reset pather's cache

    activateMapEntities(currentMapLock->map);


    if (!currentMapLock->update())
        NazaraError("Cannot update map");

    pos.x = x;
    pos.y = y;

    mapPos.x = mapX;
    mapPos.y = mapY;

    if (p->HasComponent<OrientationComponent>())
        p->GetComponent<OrientationComponent>().dir = newOrient;

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

bool isMapUtilityInited()
{
    return !m_currentMap.expired() && !m_pather.expired() && m_mainChar.IsValid();
}

std::queue<AbsTile> directionsToPositions(std::queue<std::pair<DirectionFlags, bool>> directions, AbsTile start)
{
    std::queue<AbsTile> positions;

    while (!directions.empty())
    {
        auto& dir = directions.front();
        auto xy = DirToXY(dir.first);

        start.x += xy.x;
        start.y += xy.y;

        positions.push(start);
        directions.pop();
    }

    return positions;
}
