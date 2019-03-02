// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
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


const TileArray& MapData::getTiles() const
{
    return m_tiles;
}

void MapData::setTiles(const TileArray& tiles)
{
    m_tiles = tiles;
}

const TileData& MapData::getTile(unsigned x, unsigned y) const
{
    return m_tiles[XYToIndex(x, y)];
}

const TileData& MapData::getTile(unsigned index) const
{
    return m_tiles[index];
}

std::unordered_map<Nz::Vector2ui, TileData> MapData::adjacentTiles(unsigned index)
{
    auto pos = IndexToXY(index);
    return adjacentTiles(pos.first, pos.second);
}


const Ndk::EntityList& MapData::getGraphicalEntities() const
{
    return m_graphicalEntities;
}

Ndk::EntityList& MapData::getGraphicalEntities()
{
    return m_graphicalEntities;
}

const Ndk::EntityList& MapData::getMonsterGroups() const
{
    return m_monsterGroups;
}

Ndk::EntityList& MapData::getMonsterGroups()
{
    return m_monsterGroups;
}
