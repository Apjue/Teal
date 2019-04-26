// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Core/Flags.hpp>
#include <Nazara/Core/String.hpp>
#include "util/underlyingtype.hpp"
#include "util/util.hpp"
#include "def/gamedef.hpp"

using AbsTile = Nz::Vector2ui; // Absolute Tile (0-15, 0-8)
using DiffTile = Nz::Vector2i; // Difference Tile (from one point to another one)

enum class Orientation
{
    Down,
    DownRight,
    Right,
    UpRight,
    Up,
    UpLeft,
    Left,
    DownLeft
};

inline Orientation stringToOrientation(Nz::String string);
inline Nz::String orientationToString(Orientation orientation);

enum class Direction
{
    Up,
    Down,
    Left,
    Right,

    Max = Right
};

namespace Nz
{

template<>
struct EnumAsFlags<Direction>
{
    static constexpr bool value = true;
    static constexpr int  max = toUnderlyingType<Direction>(Direction::Max);
};

}

using DirectionFlags = Nz::Flags<Direction>;

namespace Dir
{

constexpr DirectionFlags Up = Direction::Up;
constexpr DirectionFlags Down = Direction::Down;
constexpr DirectionFlags Left = Direction::Left;
constexpr DirectionFlags Right = Direction::Right;

constexpr DirectionFlags UpDown = Up | Down;
constexpr DirectionFlags LeftRight = Left | Right;

constexpr DirectionFlags DownRight = Down | Right;
constexpr DirectionFlags UpRight = Up | Right;
constexpr DirectionFlags UpLeft = Up | Left;
constexpr DirectionFlags DownLeft = Down | Left;

}

inline AbsTile operator+(const AbsTile& tile, const DirectionFlags& dir);
inline AbsTile operator+=(const AbsTile& tile, const DirectionFlags& dir);

// Conversion functions
extern DiffTile DirToXY(DirectionFlags d, bool even);
extern DiffTile DirToGXY(DirectionFlags d);
extern DirectionFlags XYToDir(DiffTile d, bool even);

extern Orientation DirToOrient(DirectionFlags d);
extern DirectionFlags OrientToDir(Orientation orientation);

inline DiffTile OrientToDiff(Orientation orientation, bool even);
inline DiffTile AbsPosToDiff(const AbsTile& from, const AbsTile& to);

///
/// \fn isDiagonal
///
/// \brief Checks if a direction goes in diagonal
///        (i.e. when two bits or more are set)
///
/// \example Up | Left will return true
///          Up | Down | Left | Right too
///          But not Up
///
/// \param dir Direction to check
///
/// \return if the direction is diagonal
///

extern bool isDiagonal(DirectionFlags dir);
inline bool isPositionValid(AbsTile pos);

#include "global.inl"

#endif // GLOBAL_HPP
