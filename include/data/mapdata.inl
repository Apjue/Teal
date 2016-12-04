// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class... Args>
MapDataRef MapData::New(Args&&... args)
{
    std::unique_ptr<MapData> object(new MapData(std::forward<Args>(args)...));
    object->SetPersistent(false);

    return object.release();
}

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


void MapData::setTiles(const TILEARRAY& tiles)
{
    m_tiles = tiles;
    updateOldTileArray();
}

void MapData::setMap(const STRINGTILEARRAY& map)
{
    m_map = map;
    updateTileArray();
}

void MapData::setObs(const UNSIGNEDTILEARRAY& obs)
{
    m_obs = obs;
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
