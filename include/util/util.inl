// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

unsigned dToU(double d)
{
    return static_cast<unsigned>(d);
}

template<class T>
T distance(const T& x, const T& y)
{
    return (x > y) ? x - y : y - x;
}

DiffTile AbsPosToDiff(const AbsTile& from, const AbsTile& to)
{
    int diffX { static_cast<int>(to.x) - static_cast<int>(from.x) },
        diffY { static_cast<int>(to.y) - static_cast<int>(from.y) };

    return { diffX, diffY };
}