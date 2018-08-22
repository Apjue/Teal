// Copyright (C) 2018 Samy Bensaid
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


const Ndk::EntityList& MapData::getEntities() const
{
    return m_entities;
}

Ndk::EntityList& MapData::getEntities()
{
    return m_entities;
}

#include <NDK/LuaAPI.hpp>
#include "util/nzstlcompatibility.hpp"
#include "util/mapposutil.hpp"

namespace Nz
{

unsigned int LuaImplQueryArg(const LuaState& state, int index, MapDataRef* mapRef, TypeTag<MapDataRef>)
{
    state.CheckType(index, Nz::LuaType_Table);

    if (!mapRef->IsValid())
        (*mapRef) = MapData::New();

    MapData* map = mapRef->Get();
    TileArray tiles;

    for (int i { 1 }; i <= Def::TileArraySize; ++i)
    {
        state.PushInteger(i);
        state.GetTable(index - 1);

        TealException(state.GetType(-1) == Nz::LuaType_Table, Nz::String { "Lua: teal_map[" } + i + "] isn't a table !");

        tiles[i - 1].textureId = state.CheckField<Nz::String>("textureId");
        tiles[i - 1].fightTextureId = state.CheckField<Nz::String>("fightTextureId");

        unsigned obstacle = state.CheckField<unsigned>("obstacle");

        switch (obstacle)
        {
            case 1:
                tiles[i - 1].flags |= TileFlag::ViewObstacle;
                break;

            case 2:
                tiles[i - 1].flags |= TileFlag::BlockObstacle;
                break;
        }

        bool visible = state.CheckField<bool>("visible");

        if (!visible)
            tiles[i - 1].flags |= TileFlag::Invisible;

        state.Pop();
    }

    map->setTiles(tiles);
    map->setPosition(toVector2(stringToMapXY(state.CheckField<Nz::String>("pos", index))));

    return 1;
}

} // namespace Nz
