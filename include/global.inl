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
    using utype = typename std::underlying_type<Dir>::type;
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

} //namespace Direction

bool isPositionValid(AbsTile pos)
{
    return pos.x % 2 == pos.y % 2;
}
