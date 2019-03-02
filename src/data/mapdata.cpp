// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Nazara/Graphics/Material.hpp>
#include <unordered_map>
#include "components/characters/positioncomponent.hpp"
#include "components/characters/blocktilecomponent.hpp"
#include "def/gamedef.hpp"
#include "data/mapdata.hpp"

MapDataLibrary::LibraryMap MapData::s_library;

MapData::MapData(const TileArray& tiles_)
    : m_tiles(tiles_) {}

std::unordered_map<Nz::Vector2ui, TileData> MapData::adjacentTiles(unsigned x, unsigned y)
{
    std::unordered_map<Nz::Vector2ui, TileData> data;
    bool even = isLineEven(y);

    for (std::size_t i {}; i < Def::MapDistanceCost.size(); ++i)
    {
        unsigned newX = x + (even ? Def::MapDistanceEvenX[i] : Def::MapDistanceUnevenX[i]);
        unsigned newY = y + (even ? Def::MapDistanceEvenY[i] : Def::MapDistanceUnevenY[i]);

        if (!isPositionValid({ newX, newY }))
            continue;

        if (IndexToXY(XYToIndex(newX, newY)).second != y
            && (even ? Def::MapDistanceEvenY[i] : Def::MapDistanceUnevenY[i]) == 0)
            continue;

        unsigned index = XYToIndex(newX, newY);

        if (index > Def::TileArraySize)
            continue;

        data[{ newX, newY }] = m_tiles[index];
    }

    return data;
}

void MapData::updateOccupiedTiles()
{
    for (auto& tile : m_tiles)
        tile.occupied = false;

    for (auto& e : m_entities)
    {
        if (e->HasComponent<PositionComponent>() && e->HasComponent<BlockTileComponent>())
        {
            auto& xy = e->GetComponent<PositionComponent>().xy;
            m_tiles[XYToIndex(xy.x, xy.y)].occupied = true;
        }
    }
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

        TealException(state.GetType(-1) == Nz::LuaType_Table, Nz::String { "Lua: teal_map[" } +i + "] isn't a table !");

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
