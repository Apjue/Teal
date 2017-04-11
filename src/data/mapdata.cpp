// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "data/mapdata.hpp"

MapDataLibrary::LibraryMap MapData::s_library;

MapData::MapData(const STRINGTILEARRAY& map_, const UNSIGNEDTILEARRAY& obs_)
    : m_map(map_), m_obs(obs_)
{
    updateTileArray();
}

MapData::MapData(const TILEARRAY& tiles_)
    : m_tiles(tiles_)
{
    updateOldTileArray();
}


void MapData::updateOldTileArray()
{
    for (unsigned i {}; i < m_tiles.size(); ++i)
    {
        auto& tile = m_tiles[i];

        m_map[i] = tile.textureId;
        m_obs[i] = tile.obstacle;
    }

    //updateOccupiedTiles();
}

void MapData::updateTileArray()
{
    for (unsigned i {}; i < m_tiles.size(); ++i)
    {
        auto& tile = m_tiles[i];

        tile.textureId = m_map[i];
        tile.obstacle = m_obs[i];
    }

    //updateOccupiedTiles();
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
            unsigned x { pos.x }, y { pos.y };

            XYToArray(x, y);
            m_tiles[XYToIndex(x, y)].occupied = true;
        }
    }
}
