// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/assert.hpp"
#include "global.hpp"

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

    throw std::runtime_error { "Invalid orientation" };
}

inline Nz::String orientationToString(Orientation orientation)
{
    switch (orientation)
    {
        case Orientation::Down:
            return "down";

        case Orientation::DownRight:
            return "downright";

        case Orientation::Right:
            return "right";

        case Orientation::UpRight:
            return "upright";

        case Orientation::Up:
            return "up";

        case Orientation::UpLeft:
            return "upleft";

        case Orientation::Left:
            return "left";

        case Orientation::DownLeft:
            return "downleft";
    }

    throw std::runtime_error { "Invalid orientation" };
}


inline DiffTile OrientToDiff(Orientation orientation, bool even)
{
    return DirToXY(OrientToDir(orientation), even);
}

/*inline DiffTile AbsPosToDiff(const AbsTile& from, const AbsTile& to)
{
    int diffX { int(to.x) - int(from.x) },
        diffY { int(to.y) - int(from.y) };

    return { diffX, diffY };
}*/


inline AbsTile applyDiffTile(AbsTile from, DiffTile diff, bool& ok)
{
    AbsTile result { unsigned(int(from.x) + diff.x), unsigned(int(from.y) + diff.y) };

    if (isLineEven(from.y) || isLineEven(diff.y))
    {
        ok = isPositionValid(result);
        return result;
    }

    else
    {
        result += { 1, 0 };

        ok = isPositionValid(result);
        return result;
    }
}


inline bool isPositionValid(AbsTile pos)
{
    return (isLineEven(pos.y) ? pos.x <= Def::MapX : pos.x < Def::MapX) && pos.y <= Def::ArrayMapY;
}
