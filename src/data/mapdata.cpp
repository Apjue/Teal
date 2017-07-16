// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "data/mapdata.hpp"

MapDataLibrary::LibraryMap MapData::s_library;

MapData::MapData(const TILEARRAY& tiles_)
    : m_tiles(tiles_) {}

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


