// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TILEDATA_HPP
#define TILEDATA_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Core/Flags.hpp>
#include "util/underlyingtype.hpp"

enum class TileFlag
{
    NoFlag = 0,

    Invisible,

    ViewObstacle,
    BlockObstacle,

    RedSpawn,
    BlueSpawn,

    Max
};

namespace Nz
{

template<>
struct EnumAsFlags<TileFlag>
{
    static constexpr bool value = true;
    static constexpr int  max = toUnderlyingType<TileFlag>(TileFlag::Max);
};

}

using TileFlags = Nz::Flags<TileFlag>;


struct TileData
{
    bool occupied {}; // Occupied by a NPC or monster

    Nz::String textureId;
    Nz::String fightTextureId;

    TileFlags flags { TileFlag::NoFlag };

    // Utility functions
    inline bool isVisible()  const { return !((flags & TileFlag::Invisible) == TileFlag::Invisible); }

    inline bool isObstacle() const { return (flags & TileFlag::ViewObstacle) == TileFlag::ViewObstacle || (flags & TileFlag::BlockObstacle) == TileFlag::BlockObstacle; }
        inline bool isViewObstacle() const { return (flags & TileFlag::ViewObstacle) == TileFlag::ViewObstacle; }
        inline bool isBlockObstacle() const { return (flags & TileFlag::BlockObstacle) == TileFlag::BlockObstacle; }
    inline bool isWalkable() const { return !isObstacle() && !occupied; }

    inline bool isSpawn()    const { return (flags & TileFlag::RedSpawn) == TileFlag::RedSpawn || (flags & TileFlag::BlueSpawn) == TileFlag::BlueSpawn; }
        inline bool isRedObstacle() const { return (flags & TileFlag::RedSpawn) == TileFlag::RedSpawn; }
        inline bool isBlueObstacle() const { return (flags & TileFlag::BlueSpawn) == TileFlag::BlueSpawn; }
};

#endif // TILEDATA_HPP
