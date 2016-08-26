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
#include "gamedef.hpp"

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


inline constexpr Dir operator & (Dir a, Dir b)
{
    using utype = typename std::underlying_type<Dir>::type;
    return static_cast<Dir>( static_cast<utype>(a) & static_cast<utype>(b) );
}

inline constexpr Dir operator | (Dir a, Dir b)
{
    using utype = typename std::underlying_type<Dir>::type;
    return static_cast<Dir>( static_cast<utype>(a) | static_cast<utype>(b) );
}


extern void operator |= (Dir& a, Dir b);
extern void operator &= (Dir& a, Dir b);

extern Dir operator ~ (Dir a);

}

extern int DirToY(Direction::Dir d);
extern int DirToX(Direction::Dir d);
extern DiffTile DirToXY(Direction::Dir d);

extern Orientation DirToOrien(Direction::Dir d);
extern Direction::Dir OrienToDir(Orientation o);

extern Direction::Dir XYToDir(DiffTile d);

#endif // GLOBAL_HPP
