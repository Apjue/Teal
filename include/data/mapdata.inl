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
    bool even = isLineEven(y);

    for (std::size_t i {}; i < Def::MAP_DISTANCE_COST.size(); ++i)
    {
        unsigned newX = x + even ? Def::MAP_DISTANCE_EVEN_X[i] : Def::MAP_DISTANCE_UNEVEN_X[i];
        unsigned newY = y + even ? Def::MAP_DISTANCE_EVEN_Y[i] : Def::MAP_DISTANCE_UNEVEN_Y[i];

        if (!isPositionValid({ newX, newY }))
            continue;

        if (IndexToXY(XYToIndex(newX, newY)).second != y
        && (even ? Def::MAP_DISTANCE_EVEN_Y[i] : Def::MAP_DISTANCE_UNEVEN_Y[i]) == 0)
            continue;

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
