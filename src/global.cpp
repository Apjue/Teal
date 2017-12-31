// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Nazara/Platform/Mouse.hpp>
#include <array>
#include <stdexcept>
#include "util/assert.hpp"
#include "util/nzstlcompatibility.hpp"
#include "global.hpp"

DiffTile DirToXY(DirectionFlags d, bool even)
{
    auto pos = toUnderlyingType(DirToOrient(d));
    int x = even ? Def::MapDistanceEvenX[pos] : Def::MapDistanceUnevenX[pos];
    int y = even ? Def::MapDistanceEvenY[pos] : Def::MapDistanceUnevenY[pos];

    return { x, y };
}

DiffTile DirToGXY(DirectionFlags d)
{
    TealAssert(d, "No flag set !");
    TealAssert(d != Dir::LeftRight && d != Dir::UpDown, "Strange flags you have there");

    int x {}, y {};
    int modifier = isDiagonal(d) ? 1 : 2;

    if (d & Dir::Up)
        y -= modifier;

    if (d & Dir::Down)
        y += modifier;

    if (d & Dir::Left)
        x -= modifier;

    if (d & Dir::Right)
        x += modifier;

    return { x, y };
}

DirectionFlags XYToDir(DiffTile d, bool even)
{
    int x { d.x };
    int y { d.y };

    TealAssert((x != 0 || y != 0), "x and y may not be 0");

    auto dir = Dir::Up; // Must put a default value

    for (unsigned i {}; i < Def::MapDistanceCost.size(); ++i)
    {
        if (x == (even ? Def::MapDistanceEvenX[i] : Def::MapDistanceUnevenX[i])
         && y == (even ? Def::MapDistanceEvenY[i] : Def::MapDistanceUnevenY[i]))
            return OrientToDir(static_cast<Orientation>(i));
    }

    return dir;
}

Orientation DirToOrient(DirectionFlags d)
{
    TealAssert(d, "No flag set !");

    if ((d & Dir::DownLeft) == Dir::DownLeft)
        return Orientation::DownLeft;

    if ((d & Dir::DownRight) == Dir::DownRight)
        return Orientation::DownRight;

    if ((d & Dir::UpLeft) == Dir::UpLeft)
        return Orientation::UpLeft;

    if ((d & Dir::UpRight) == Dir::UpRight)
        return Orientation::UpRight;


    if ((d & Dir::Down) == Dir::Down)
        return Orientation::Down;

    if ((d & Dir::Up) == Dir::Up)
        return Orientation::Up;

    if ((d & Dir::Left) == Dir::Left)
        return Orientation::Left;

    if ((d & Dir::Right) == Dir::Right)
        return Orientation::Right;

    return {};
}

DirectionFlags OrientToDir(Orientation o)
{
    switch (o)
    {
    case Orientation::Down:
        return Dir::Down;

    case Orientation::Up:
        return Dir::Up;

    case Orientation::Left:
        return Dir::Left;

    case Orientation::Right:
        return Dir::Right;

    case Orientation::DownLeft:
        return Dir::DownLeft;

    case Orientation::DownRight:
        return Dir::DownRight;

    case Orientation::UpLeft:
        return Dir::UpLeft;

    case Orientation::UpRight:
        return Dir::UpRight;
    }

    TealAssert(false, "No flag set !");
    return {};
}

bool isDiagonal(DirectionFlags dir)
{
    bool diag = true;

    if (dir == Dir::Right
     || dir == Dir::Down
     || dir == Dir::Left
     || dir == Dir::Up)
        diag = false;

    return diag;
}
