// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/maputil.hpp"

namespace
{

std::weak_ptr<MapInstance> m_currentMap {};
std::weak_ptr<micropather::MicroPather> m_pather {};

}

std::pair<bool, Direction::Dir> canChangeMap(const Ndk::EntityHandle& p)
{
    NazaraAssert(isMapUtilityInited(), "Map Utility hasn't been initialized !");
    NazaraAssert(hasComponentsToChangeMap(p), "Entity hasn't right components to change map !");

    auto& mapPos = p->GetComponent<MapPositionComponent>();
    auto& pos = p->GetComponent<PositionComponent>();

    // Where is the entity in the map ? Right, left, down, or up ?

    bool validPos { false }; // If the entity is at one extremity
    Direction::Dir entExt { Direction::Up }; // Entity Extremity

    if (pos.x == 0u) // Left
    {
        if (MapDataLibrary::Has(mapXYToString(mapPos.x - 1, mapPos.y)))
        {
            validPos = true;
            entExt = Direction::Left;
        }
    }

    else if (pos.x == Def::LMAPX) // Right
    {
        if (MapDataLibrary::Has(mapXYToString(mapPos.x + 1, mapPos.y)))
        {
            validPos = true;
            entExt = Direction::Right;
        }
    }

    else if (pos.y == 0u) // Up
    {
        if (MapDataLibrary::Has(mapXYToString(mapPos.x, mapPos.y - 1)))
        {
            validPos = true;
            entExt = Direction::Up;
        }
    }

    else if (pos.y == Def::LMAPY) // Down
    {
        if (MapDataLibrary::Has(mapXYToString(mapPos.x, mapPos.y + 1)))
        {
            validPos = true;
            entExt = Direction::Down;
        }
    }

    if (!validPos)
        return std::make_pair(false, entExt); // Entity isn't even at an extremity

    // Okay, now, let's check if the position where the entity
    // will move to is valid (no obstacle)

    MapDataRef map; // Map the entity will move to
    unsigned x {}, y {}; // New position of the entity after changing map

    switch (entExt)
    {
    case Direction::Left:
        map = MapDataLibrary::Get(mapXYToString(mapPos.x - 1, mapPos.y));

        x = Def::LMAPX;
        y = pos.y;

        break;

    case Direction::Right:
        map = MapDataLibrary::Get(mapXYToString(mapPos.x + 1, mapPos.y));

        x = 0u;
        y = pos.y;

        break;

    case Direction::Up:
        map = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y - 1));

        x = pos.x;
        y = Def::LMAPY;

        break;

    case Direction::Down:
        map = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y + 1));

        x = pos.x;
        y = 0u;

        break;

    default:
        NazaraError("Bad direction value");

        return std::make_pair(false, entExt);
        break;
    }

    NazaraAssert(map, "new map null !");

    MapInstance::XYToArray(x, y);

    if (map->obs()[x + y * Def::MAPX] != 0)
        return std::make_pair(false, entExt); // It's an obstacle.

    return std::make_pair(true, entExt);
}

bool changeMap(const Ndk::EntityHandle& p)
{
    auto canChange = canChangeMap(p);

    if (!canChange.first)
        return false;

    auto& mapPos = p->GetComponent<MapPositionComponent>();
    auto& pos = p->GetComponent<PositionComponent>();

    MapDataRef newMap; // Map the entity will move to
    unsigned x {}, y {}; // New position of the entity after changing map
    unsigned mapX {}, mapY {}; // Position of the new map
    Orientation newOrient { Orientation::Down };

    switch (canChange.second)
    {
    case Direction::Left:
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x - 1, mapPos.y));

        x = Def::LMAPX;
        y = pos.y;

        mapX = mapPos.x - 1;
        mapY = mapPos.y;

        newOrient = Orientation::Left;

        break;

    case Direction::Right:
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x + 1, mapPos.y));

        x = 0u;
        y = pos.y;

        mapX = mapPos.x + 1;
        mapY = mapPos.y;

        newOrient = Orientation::Right;

        break;

    case Direction::Up:
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y - 1));

        x = pos.x;
        y = Def::LMAPY;

        mapX = mapPos.x;
        mapY = mapPos.y - 1;

        newOrient = Orientation::Up;

        break;

    case Direction::Down:
        newMap = MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y + 1));

        x = pos.x;
        y = 0u;

        mapX = mapPos.x;
        mapY = mapPos.y + 1;

        newOrient = Orientation::Down;

        break;
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
                    const std::weak_ptr<micropather::MicroPather>& pather)
{
    m_currentMap = currentMap;
    m_pather = pather;
}

bool isMapUtilityInited()
{
    return !m_currentMap.expired() && !m_pather.expired();
}
