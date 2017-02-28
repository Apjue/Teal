// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "global.hpp"

int DirToY(DirectionFlags d)
{
    NazaraAssert((d & Dir::UpDown) != Dir::UpDown, "Cannot go up and down" );

    static constexpr std::array<int, 4> moves { 0, -1, 1, 0 };
    auto newDir = d & Dir::UpDown;

    if (newDir & Dir::Up)
        return -1;

    if (newDir & Dir::Down)
        return 1;

    return 0;
}

int DirToX(DirectionFlags d)
{
    NazaraAssert((d & Dir::LeftRight) != Dir::LeftRight, "Cannot go left and right");

    static constexpr std::array<int, 4> moves { 0, -1, 1, 0 };
    auto newDir = d & Dir::LeftRight;

    if (newDir & Dir::Left)
        return -1;

    if (newDir & Dir::Right)
        return 1;

    return 0;
}

DiffTile DirToXY(DirectionFlags d)
{
    int const x = DirToX(d);
    int const y = DirToY(d);

    return { y ? x : x*2, x ? y : y*2 };
}

DirectionFlags XYToDir(DiffTile d)
{
    int x { d.x };
    int y { d.y };

    NazaraAssert((x != 0 || y != 0), "x and y may not be 0");
    // 0 == no move == no direction

    auto dir = Dir::Up; // Must put a default value

    if (x > 0)
        dir |= Dir::Right;
    else if (x < 0)
        dir |= Dir::Left;

    if (y > 0)
        dir |= Dir::Down;
    if (y >= 0)
        dir &= ~Dir::Up;

    return dir;
}

Orientation DirToOrient(DirectionFlags d)
{
    NazaraAssert(d, "No flag set !");

    if (d & Dir::DownLeft == Dir::DownLeft)
        return Orientation::DownLeft;

    if (d & Dir::DownRight == Dir::DownRight)
        return Orientation::DownRight;

    if (d & Dir::UpLeft == Dir::UpLeft)
        return Orientation::UpLeft;

    if (d & Dir::UpRight == Dir::UpRight)
        return Orientation::UpRight;


    if (d & Dir::Down == Dir::Down)
        return Orientation::Down;

    if (d & Dir::Up == Dir::Up)
        return Orientation::Up;

    if (d & Dir::Left == Dir::Left)
        return Orientation::Left;

    if (d & Dir::Right == Dir::Right)
        return Orientation::Right;

    return {}; // Oh, well...
}

DirectionFlags OrienToDir(Orientation o)
{
    switch (o)
    {
    case Orientation::Down:
        return Dir::Down;
        break;

    case Orientation::Up:
        return Dir::Up;
        break;

    case Orientation::Left:
        return Dir::Left;
        break;

    case Orientation::Right:
        return Dir::Right;
        break;

    case Orientation::DownLeft:
        return Dir::DownLeft;
        break;

    case Orientation::DownRight:
        return Dir::DownRight;
        break;

    case Orientation::UpLeft:
        return Dir::UpLeft;
        break;

    case Orientation::UpRight:
        return Dir::UpRight;
        break;
    }

    NazaraAssert(false, "No flag set !");
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
