// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class... Args>
MapDataRef MapData::New(Args&&... args)
{
    std::unique_ptr<MapData> object(new MapData(std::forward<Args>(args)...));
    object->SetPersistent(false);

    return object.release();
}


inline const Nz::Vector2i& MapData::getPosition() const
{
    return m_pos;
}

inline void MapData::setPosition(const Nz::Vector2i& pos)
{
    m_pos = pos;
}


const TileArray& MapData::tiles() const
{
    return m_tiles;
}

const TileData& MapData::tile(unsigned x, unsigned y) const
{
    return m_tiles[XYToIndex(x, y)];
}

const TileData& MapData::tile(unsigned index) const
{
    return m_tiles[index];
}

void MapData::setTiles(const TileArray& tiles)
{
    m_tiles = tiles;
}


const Ndk::EntityList& MapData::getEntities() const
{
    return m_entities;
}

Ndk::EntityList& MapData::getEntities()
{
    return m_entities;
}
