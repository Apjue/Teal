// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

const TILEARRAY& MapData::tiles() const
{
    return m_tiles;
}

const STRINGTILEARRAY& MapData::map() const
{
    return m_map;
}

const UNSIGNEDTILEARRAY& MapData::obs() const
{
    return m_obs;
}


void MapData::setTiles(const TILEARRAY& nTiles)
{
    m_tiles = nTiles;
    updateOldTileArray();
}

void MapData::setMap(const STRINGTILEARRAY& nMap)
{
    m_map = nMap;
    updateTileArray();
}

void MapData::setObs(const UNSIGNEDTILEARRAY& nObs)
{
    m_obs = nObs;
    updateTileArray();
}


void MapData::addEntity(const Ndk::EntityHandle& e)
{
    m_entities.Insert(e);
}

const Ndk::EntityList& MapData::getEntities() const
{
    return m_entities;
}

Ndk::EntityList& MapData::getEntities()
{
    return m_entities;
}
