// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "global.hpp"

namespace Direction
{

void operator |= (Dir& a, Dir b)
{
    a = a | b;
}

void operator &= (Dir& a, Dir b)
{
    a = a & b;
}

Dir operator ~ (Dir a)
{
    auto const nbFlags = Def::MAXDIR;
    using utype = typename std::underlying_type<Dir>::type;
    return static_cast<Dir>( ~static_cast<utype>(a) & ( ( ( utype(1u) << nbFlags ) - 1u ) ) );
}

}

int DirToY(Direction::Dir d)
{
    assert( ( d & Direction::UpDown) != Direction::UpDown );
    static constexpr std::array<int, 4> moves {0,-1,1,0};
    return moves[ d & Direction::UpDown];
}

int DirToX(Direction::Dir d)
{
    assert( ( d & Direction::LeftRight) != Direction::LeftRight );
    static constexpr std::array<int, 4> moves {0,-1,1,0};
    return moves[( d & Direction::LeftRight)/4];
}

DiffTile DirToXY(Direction::Dir d)
{
    auto const x = DirToX(d);
    auto const y = DirToY(d);

    return {y ? x : x*2, x ? y : y*2};
}

Orientation DirToOrien(Direction::Dir d)
{
    assert(d && "No flag set !");

    if ( (d & Direction::DownLeft) == Direction::DownLeft)
        return Orientation::DownLeft;

    if ( (d & Direction::DownRight) == Direction::DownRight)
        return Orientation::DownRight;

    if ( (d & Direction::UpLeft) == Direction::UpLeft)
        return Orientation::UpLeft;

    if ( (d & Direction::UpRight) == Direction::UpRight)
        return Orientation::UpRight;


    if ( (d & Direction::Down) == Direction::Down)
        return Orientation::Down;

    if ( (d & Direction::Up) == Direction::Up)
        return Orientation::Up;

    if ( (d & Direction::Left) == Direction::Left)
        return Orientation::Left;

    if ( (d & Direction::Right) == Direction::Right)
        return Orientation::Right;

    return {};
}

Direction::Dir OrienToDir(Orientation o)
{
    switch (o)
    {
    case Orientation::Down:
        return Direction::Down;
        break;

    case Orientation::Up:
        return Direction::Up;
        break;

    case Orientation::Left:
        return Direction::Left;
        break;

    case Orientation::Right:
        return Direction::Right;
        break;

    case Orientation::DownLeft:
        return Direction::DownLeft;
        break;

    case Orientation::DownRight:
        return Direction::DownRight;
        break;

    case Orientation::UpLeft:
        return Direction::UpLeft;
        break;

    case Orientation::UpRight:
        return Direction::UpRight;
        break;
    }

    assert(!"No flag set !");
    return {};
}

Direction::Dir XYToDir(DiffTile d)
{
    int x { d.first() };
    int y { d.second() };

    assert( (x != 0 || y != 0) && "X and Y may not be 0" );
    //0 == no move == no direction

    Direction::Dir dir = Direction::Up; //Must put a default value

    if (x > 0)
        dir |= Direction::Right;
    else if (x < 0)
        dir |= Direction::Left;

    if (y > 0)
        dir |= Direction::Down;
    if (y >= 0)
        dir &= ~Direction::Up;

    return dir;
}
