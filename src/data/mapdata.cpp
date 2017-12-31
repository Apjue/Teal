// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Nazara/Graphics/Material.hpp>
#include <unordered_map>
#include "components/common/positioncomponent.hpp"
#include "components/common/blocktilecomponent.hpp"
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
        unsigned newX = x + even ? Def::MapDistanceEvenX[i] : Def::MapDistanceUnevenX[i];
        unsigned newY = y + even ? Def::MapDistanceEvenY[i] : Def::MapDistanceUnevenY[i];

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

    for (auto& e : m_entities)
    {
        if (e->HasComponent<PositionComponent>() && e->HasComponent<BlockTileComponent>()
            && e->GetComponent<BlockTileComponent>().blockTile)
        {
            auto& pos = e->GetComponent<PositionComponent>();
            m_tiles[XYToIndex(pos.xy.x, pos.xy.y)].occupied = true;
        }
    }
}


