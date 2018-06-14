// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/assert.hpp"
#include "global.hpp"

inline AbsTile operator+(const AbsTile& tile, const DirectionFlags& dir)
{
    DiffTile diff = DirToXY(dir, isLineEven(tile.y));

    if (!(diff.x >= 0 || int(tile.x) >= -diff.x) || !(diff.y >= 0 || int(tile.y) >= -diff.y))
        return tile;

    return AbsTile { tile.x + diff.x, tile.y + diff.y };
}

inline AbsTile operator+=(const AbsTile& tile, const DirectionFlags& dir)
{
    return AbsTile { tile + dir };
}

inline Orientation stringToOrientation(Nz::String string)
{
    string = string.ToLower();

    if (string == "down")
        return Orientation::Down;

    if (string == "downleft")
        return Orientation::DownLeft;

    if (string == "downright")
        return Orientation::DownRight;

    if (string == "left")
        return Orientation::Left;

    if (string == "right")
        return Orientation::Right;

    if (string == "up")
        return Orientation::Up;

    if (string == "upleft")
        return Orientation::UpLeft;

    if (string == "upright")
        return Orientation::UpRight;

    return {};
}


inline DiffTile OrientToDiff(Orientation orientation, bool even)
{
    return DirToXY(OrientToDir(orientation), even);
}

inline DiffTile AbsPosToDiff(const AbsTile& from, const AbsTile& to)
{
    int diffX { int(to.x) - int(from.x) },
        diffY { int(to.y) - int(from.y) };

    return { diffX, diffY };
}

inline bool isPositionValid(AbsTile pos)
{
    if (pos.x > Def::ArrayMapX || pos.y > Def::ArrayMapY)
        return false;

    return (isLineEven(pos.y) ? pos.x <= Def::MapX : pos.x < Def::MapX) && pos.y <= Def::ArrayMapY;
}
