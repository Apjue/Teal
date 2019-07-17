// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Nazara/Graphics/Material.hpp>
#include <unordered_map>
#include "components/shared/positioncomponent.hpp"
#include "components/shared/blocktilecomponent.hpp"
#include "def/gamedef.hpp"
#include "data/mapdata.hpp"

MapDataLibrary::LibraryMap MapData::s_library;

MapData::MapData(const TileArray& tiles_)
    : m_tiles(tiles_) {}

std::unordered_map<Nz::Vector2ui, TileData> MapData::adjacentTiles(unsigned x, unsigned y)
{
    std::unordered_map<Nz::Vector2ui, TileData> data;
    bool even = isLineEven(y);

    for (std::size_t i {}; i < Def::MapDistanceCost.size(); ++i)
    {
        unsigned newX = x + (even ? Def::MapDistanceEvenX[i] : Def::MapDistanceUnevenX[i]);
        unsigned newY = y + (even ? Def::MapDistanceEvenY[i] : Def::MapDistanceUnevenY[i]);

        if (!isPositionValid({ newX, newY }))
            continue;

        if (IndexToXY(XYToIndex(newX, newY)).second != y
            && (even ? Def::MapDistanceEvenY[i] : Def::MapDistanceUnevenY[i]) == 0)
            continue;

        unsigned index = XYToIndex(newX, newY);

        if (index > Def::TileArraySize)
            continue;

        data[{ newX, newY }] = m_tiles[index];
    }

    return data;
}

void MapData::updateOccupiedTiles()
{
    for (auto& tile : m_tiles)
        tile.occupied = false;

    for (auto& e : m_graphicalEntities)
    {
        if (e->HasComponent<PositionComponent>() &&
            e->HasComponent<BlockTileComponent>() && e->GetComponent<BlockTileComponent>().active)
        {
            auto& blockTile = e->GetComponent<BlockTileComponent>();
            auto& xy = e->GetComponent<PositionComponent>().xy;

            if (blockTile.occupied.empty())
                m_tiles[XYToIndex(xy.x, xy.y)].occupied = true;

            else
                for (DiffTile& tile : blockTile.occupied)
                {
                    bool ok = false;
                    AbsTile resultTile = applyDiffTile(xy, tile, ok);

                    if (!ok)
                        continue; // Out of bounds

                    m_tiles[XYToIndex(resultTile.x, resultTile.y)].occupied = true;
                }
        }
    }
}
