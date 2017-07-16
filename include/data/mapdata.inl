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


const TILEARRAY& MapData::tiles() const
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

std::unordered_map<Nz::Vector2ui, TileData> MapData::adjacentTiles(unsigned x, unsigned y)
{
    std::unordered_map<Nz::Vector2ui, TileData> data;

    for (std::size_t i {}; i < Def::MAP_DISTANCE_X.size(); ++i)
    {
        unsigned newX = x + Def::MAP_DISTANCE_X[i]; // todo: bugfix: array +1 => other line
        unsigned newY = y + Def::MAP_DISTANCE_Y[i]; // COORDFIX_REDO

        unsigned index = XYToIndex(newX, newY);

        if (index > Def::TILEARRAYSIZE)
            continue;

        data[{ newX, newY }] = m_tiles[index];
    }

    return data;
}

void MapData::setTiles(const TILEARRAY& tiles)
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
