// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "data/mapdata.hpp"

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
}

void MapData::updateTileArray()
{
    for (unsigned i {}; i < m_tiles.size(); ++i)
    {
        auto& tile = m_tiles[i];

        tile.textureId = m_map[i];
        tile.obstacle = m_obs[i];
    }
}
