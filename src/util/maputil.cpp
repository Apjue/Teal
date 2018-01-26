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
#include "util/mathutil.hpp"
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

std::vector<AbsTile> getVisibleTiles(AbsTile pos, unsigned range, bool viewThroughObstacles, bool includeObstacles)
{
    if (range == 0)
        return {}; // but why

    MapDataRef map = getCurrentMap()->getCurrentMap();
    std::vector<AbsTile> tilesInRange;

    // Detect tiles around, using range
    for (unsigned i {}; i < Def::TileArraySize; ++i)
    {
        auto xy = IndexToXY(i);
        unsigned distance = xy.first + (xy.second % 2 == 0 ? xy.second : xy.second + 1) / 2;
        
        if (distance <= range) // jackpot
            tilesInRange.push_back(toVector2(xy));
    }

    std::vector<AbsTile> obstacles;
    std::vector<AbsTile> passableTiles;

    for (auto& xy : tilesInRange)
    {
        const TileData& tileData = map->getTile(xy.x, xy.y);

        if (!tileData.isVisible())
            continue;

        if (!tileData.isObstacle())
            passableTiles.push_back(xy);

        else
            obstacles.push_back(xy);
    }

    if (viewThroughObstacles)
    {
        if (!includeObstacles)
            return passableTiles;

        else
        {
            std::vector<AbsTile> obstaclesAndPassables;

            for (auto& tile : obstacles)
                obstaclesAndPassables.push_back(tile);

            for (auto& tile : passableTiles)
                obstaclesAndPassables.push_back(tile);

            return obstaclesAndPassables;
        }
    }

    // It's better to use this function than to search a tile in the passableTiles vector
    auto isTilePassable = [&map] (unsigned x, unsigned y) { return !map->getTile(x, y).isObstacle(); };
    std::vector<AbsTile> visibleTiles = passableTiles;

    for (auto& obstacle : obstacles) // Todo optimize this ? Make some blocks of obstacles to compare with fewer rays
    {
        Vector2fTriplet rays { getTileCenter(pos.x, pos.y) };
        bool extremity1filled { false };

        for (unsigned i {}; i < Def::TileVertexNumber; ++i) // Detect correct extremities
        {
            bool isExtremity { true };
            Nz::Ternary left = Nz::Ternary_Unknown;

            for (unsigned j {}; j < Def::TileVertexNumber; ++j)
            {
                if (j == i)
                    continue;

                Direction dir = static_cast<Direction>(j);
                bool atLeft = isLeft(rays.first, getTileVertex(static_cast<Direction>(i), obstacle.x, obstacle.y), getTileVertex(dir, obstacle.x, obstacle.y));

                if (left == Nz::Ternary_Unknown)
                    left = atLeft ? Nz::Ternary_True : Nz::Ternary_False;

                if (left != (atLeft ? Nz::Ternary_True : Nz::Ternary_False))
                {
                    isExtremity = false;
                    break;
                }
            }

            if (isExtremity)
            {
                if (!extremity1filled)
                {
                    rays.second = getTileVertex(static_cast<Direction>(i), obstacle.x, obstacle.y);
                    extremity1filled = true;
                }

                else
                {
                    rays.third = getTileVertex(static_cast<Direction>(i), obstacle.x, obstacle.y);
                    break;
                }
            }
        }

        TealAssert(rays.second != Nz::Vector2f {} && rays.third != Nz::Vector2f  {}, "Triangle not completely filled");

        // Swap triangle's vertex: rays.second must be at the left, rays.third must be at the right
        if (!isRight(rays.first, rays.second, rays.third))
            std::swap(rays.second, rays.third);

        // Check shadowed tiles
        for (unsigned i {}; i < passableTiles.size(); ++i)
        {
            auto xy = IndexToXY(i);
            TealAssert(isTilePassable(xy.first, xy.second), "Unpassable tile in a passable tiles vector ?");

            // Is tile contained in obstacle shadow ?
            if (isRight(rays.first, rays.second, getTileCenter(xy.first, xy.second)) && isLeft(rays.first, rays.third, getTileCenter(xy.first, xy.second)))
                visibleTiles.erase(std::find(visibleTiles.begin(), visibleTiles.end(), toVector2(xy)));
        }
    }

    if (!includeObstacles)
        return visibleTiles;

    else
    {
        std::vector<AbsTile> obstaclesAndVisibles;

        for (auto& tile : obstacles)
            obstaclesAndVisibles.push_back(tile);

        for (auto& tile : visibleTiles)
            obstaclesAndVisibles.push_back(tile);

        return obstaclesAndVisibles;
    }
}

Vector2fPair getTileCornerSegment(Orientation corner, unsigned x, unsigned y)
{
    Nz::Rectf aabb = getTileAABB(x, y);

    switch (corner)
    {
        case Orientation::Down:
            return { Nz::Vector2f { aabb.x, aabb.y + aabb.height },
                     Nz::Vector2f { aabb.x + aabb.width, aabb.y + aabb.height } };

        case Orientation::DownRight:
            return  { Nz::Vector2f { aabb.x + aabb.width / 2, aabb.y + aabb.height },
                      Nz::Vector2f { aabb.x + aabb.width, aabb.y + aabb.height / 2 } };

        case Orientation::Right:
            return  { Nz::Vector2f { aabb.x + aabb.width, aabb.y + aabb.height },
                      Nz::Vector2f { aabb.x + aabb.width, aabb.y} };

        case Orientation::UpRight:
            return  { Nz::Vector2f { aabb.x + aabb.width, aabb.y + aabb.height / 2 },
                      Nz::Vector2f { aabb.x + aabb.width / 2, aabb.y } };

        case Orientation::Up:
            return  { Nz::Vector2f { aabb.x + aabb.width, aabb.y },
                      Nz::Vector2f { aabb.x, aabb.y } };

        case Orientation::UpLeft:
            return  { Nz::Vector2f { aabb.x + aabb.width / 2, aabb.y },
                      Nz::Vector2f { aabb.x, aabb.y + aabb.height / 2 } };

        case Orientation::Left:
            return  { Nz::Vector2f { aabb.x, aabb.y },
                      Nz::Vector2f { aabb.x, aabb.y + aabb.height } };

        case Orientation::DownLeft:
            return  { Nz::Vector2f { aabb.x, aabb.y + aabb.height / 2 },
                      Nz::Vector2f { aabb.x + aabb.width / 2, aabb.y + aabb.height } };
    }

    throw std::runtime_error { "Corner not initialized properly" };
}

Nz::Vector2f getTileVertex(Direction vertex, unsigned x, unsigned y)
{
    Nz::Rectf aabb = getTileAABB(x, y);

    switch (vertex)
    {
        case Direction::Up:
            return { aabb.x + aabb.width / 2, aabb.y };

        case Direction::Down:
            return { aabb.x + aabb.width / 2, aabb.y + aabb.height };

        case Direction::Left:
            return { aabb.x, aabb.y + aabb.height / 2 };

        case Direction::Right:
            return { aabb.x + aabb.width, aabb.y + aabb.height / 2 };
    }

    throw std::runtime_error { "Initialize your arguments next time..." };
}

/*#include <cmath> //todo: understand all this

typedef unsigned int uint;

static int multipliers[4][8] = {
    { 1, 0, 0, -1, -1, 0, 0, 1 },
    { 0, 1, -1, 0, 0, -1, 1, 0 },
    { 0, 1, 1, 0, 0, -1, -1, 0 },
    { 1, 0, 0, 1, -1, 0, 0, -1 }
};


class Map
{
public:
    Map() {};

    void set_visible(uint x, uint y, bool visible)
    {
        // Set the visibility of the cell at the given position.
    }

    uint get_width() const
    {
        // Return the width of the map.
        return 0;
    }

    uint get_height() const
    {
        // Return the height of the map.
        return 0;
    }

    bool is_opaque(uint x, uint y) const
    {
        // Return whether the given position holds an opaque cell.
        return false;
    }
};

void cast_light(Map& map, uint x, uint y, uint radius, uint row,
                float start_slope, float end_slope, uint xx, uint xy, uint yx,
                uint yy)
{
    if (start_slope < end_slope)
        return;

    float next_start_slope = start_slope;

    for (uint i = row; i <= radius; i++)
    {
        bool blocked = false;

        for (int dx = -i, dy = -i; dx <= 0; dx++)
        {
            float l_slope = (dx - 0.5) / (dy + 0.5);
            float r_slope = (dx + 0.5) / (dy - 0.5);

            if (start_slope < r_slope)
                continue;

            else if (end_slope > l_slope)
                break;

            int sax = dx * xx + dy * xy;
            int say = dx * yx + dy * yy;

            if ((sax < 0 && (uint) std::abs(sax) > x) ||
                (say < 0 && (uint) std::abs(say) > y))
                continue;

            uint ax = x + sax;
            uint ay = y + say;

            if (ax >= map.get_width() || ay >= map.get_height())
                continue;

            uint radius2 = radius * radius;

            if ((uint) (dx * dx + dy * dy) < radius2)
                map.set_visible(ax, ay, true);

            if (blocked)
            {
                if (map.is_opaque(ax, ay))
                {
                    next_start_slope = r_slope;
                    continue;
                }

                else
                {
                    blocked = false;
                    start_slope = next_start_slope;
                }
            }

            else if (map.is_opaque(ax, ay))
            {
                blocked = true;
                next_start_slope = r_slope;
                cast_light(map, x, y, radius, i + 1, start_slope, l_slope, xx,
                           xy, yx, yy);
            }
        }

        if (blocked)
            break;
    }
}

void do_fov(Map& map, uint x, uint y, uint radius)
{
    for (uint i = 0; i < 8; i++)
    {
        cast_light(map, x, y, radius, 1, 1.0, 0.0, multipliers[0][i],
                   multipliers[1][i], multipliers[2][i], multipliers[3][i]);
    }
}*/
