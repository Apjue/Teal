// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

const TILEARRAY& MapData::tiles() const
{
    return m_tiles;
}

const OLDTILEARRAY& MapData::map() const
{
    return m_map;
}

const OLDTILEARRAY& MapData::obs() const
{
    return m_obs;
}


void MapData::setTiles(const TILEARRAY& nTiles)
{
    m_tiles = nTiles;
    updateOldTileArray();
}

void MapData::setMap(const OLDTILEARRAY& nMap)
{
    m_map = nMap;
    updateTileArray();
}

void MapData::setObs(const OLDTILEARRAY& nObs)
{
    m_obs = nObs;
    updateTileArray();
}
