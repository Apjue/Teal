// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TILEDATA_HPP
#define TILEDATA_HPP

#include <Nazara/Core/String.hpp>

enum class TileFlag
{
    NoFlag,

    Invisible,

    ViewObstacle,
    BlockObstacle,

    RedSpawn,
    BlueSpawn
};

inline TileFlag stringToTileFlag(Nz::String string);
inline Nz::String tileFlagToString(TileFlag flag);

struct TileData
{
    bool occupied {}; // Occupied by a NPC or monster

    unsigned textureId;
    TileFlag flag { TileFlag::NoFlag };

    // Utility functions
    inline bool isVisible() const;

    inline bool isObstacle() const;
    inline bool isViewObstacle() const;
    inline bool isBlockObstacle() const;
    inline bool isWalkable() const;

    inline bool isSpawn() const;
    inline bool isRedSpawn() const;
    inline bool isBlueSpawn() const;
};

#include "tiledata.inl"

#endif // TILEDATA_HPP
