// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_MAPENTITYDATA_HPP
#define TEAL_MAPENTITYDATA_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include "components/shared/blocktilecomponent.hpp"

struct MapEntityData // Contains all required attributes of a map entity. Used by a map entity factory.
{
    Nz::String codename;
    Nz::SpriteRef sprite;
    Nz::Vector2f offset;
    BlockTileData blockTile;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, MapEntityData* data, TypeTag<MapEntityData>);

} // namespace Nz

#include "mapentitydata.inl"

#endif // TEAL_MAPENTITYDATA_HPP
