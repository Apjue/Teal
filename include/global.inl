// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

namespace Direction 
{

constexpr Dir operator&(Dir a, Dir b)
{
    return static_cast<Dir>(static_cast<DirUnderlyingType>(a) & static_cast<DirUnderlyingType>(b));
}

constexpr Dir operator|(Dir a, Dir b)
{
    return static_cast<Dir>(static_cast<DirUnderlyingType>(a) | static_cast<DirUnderlyingType>(b));
}


void operator|=(Dir& a, Dir b)
{
    a = a | b;
}

void operator&=(Dir& a, Dir b)
{
    a = a & b;
}

constexpr Dir operator~(Dir a)
{
    // Def::MAXDIR == number of flags
    return static_cast<Dir>(~static_cast<DirUnderlyingType>(a) &
        (((DirUnderlyingType(1u) << Def::MAXDIR) - 1u)));
}

} //namespace Direction

bool isPositionValid(AbsTile pos)
{
    return pos.x % 2 == pos.y % 2;
}
