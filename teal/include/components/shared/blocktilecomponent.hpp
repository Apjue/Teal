// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef BLOCKTILECOMPONENT_HPP
#define BLOCKTILECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <vector>
#include "global.hpp"

struct BlockTileData;

struct BlockTileComponent : public Ndk::Component<BlockTileComponent>
{
    // DiffTiles, relative to the center (pos in PositionComponent)
    std::vector<DiffTile> occupied; // If the entity only occupies the tile set in PositionComponent, this vector is left empty
                                    // Else, it must contain all the tiles, including the one in PositionComponent
    bool active = true;             

    static Ndk::ComponentIndex componentIndex;
};

struct BlockTileData
{
    bool use;
    BlockTileComponent data;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, BlockTileData* data, TypeTag<BlockTileData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    data->use = state.CheckField<bool>("blocktile", false, index);

    if (state.GetField("blocktiles", index) == Nz::LuaType_Table)
    {
        for (int i { 1 };; ++i)
        {
            state.PushInteger(i);

            if (state.GetTable() != Nz::LuaType_Table)
            {
                state.Pop();
                break;
            }

            int localIndex = -1;
            data->data.occupied.push_back(state.Check<DiffTile>(&localIndex));

            state.Pop();
        }
    }

    state.Pop();

    return 1;
}

} // namespace Nz

#endif // BLOCKTILECOMPONENT_HPP
