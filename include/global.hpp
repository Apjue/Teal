// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <Nazara/Utility/Mouse.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <type_traits>
#include <assert.h>
#include <stdexcept>
#include "def/gamedef.hpp"

using Button = Nz::Mouse::Button;

using AbsTile = Nz::Vector2ui; // Absolute Tile (0-15, 0-8)
using DiffTile = Nz::Vector2i; // Difference Tile (can be negative or positive)

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

namespace Direction
{

enum Direction
{
    Up = 1<<0,
    Down = 1<<1,
    Left = 1<<2,
    Right = 1<<3,

    UpDown = Up | Down,
    LeftRight = Left | Right,

    DownRight = Down | Right,
    UpRight = Up | Right,
    UpLeft = Up | Left,
    DownLeft = Down | Left
};

using Dir = Direction;
using DirUnderlyingType = typename std::underlying_type<Dir>::type;

inline constexpr Dir operator&(Dir a, Dir b);
inline constexpr Dir operator|(Dir a, Dir b);


inline void operator|=(Dir& a, Dir b);
inline void operator&=(Dir& a, Dir b);

extern Dir operator~(Dir a);

}

//Conversion functions
extern int DirToY(Direction::Dir d);
extern int DirToX(Direction::Dir d);

extern DiffTile DirToXY(Direction::Dir d);
extern Direction::Dir XYToDir(DiffTile d);

extern Orientation DirToOrien(Direction::Dir d);
extern Direction::Dir OrienToDir(Orientation o);

///
/// \fn isDiagonal
///
/// \brief Checks if a direction goes in diagonal
///        (i.e. when two bits are set)
///
/// \param dir Direction to check
///

extern bool isDiagonal(Direction::Dir dir);
inline bool isPositionValid(AbsTile pos);

#include "global.inl"

#endif // GLOBAL_HPP

