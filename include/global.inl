// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

Orientation stringToOrientation(Nz::String string)
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


DiffTile OrientToDiff(Orientation o, bool even)
{
    return DirToXY(OrientToDir(o), even);
}

bool isPositionValid(AbsTile pos)
{
    if (pos.x > Def::ARRAYMAPX || pos.y > Def::ARRAYMAPY)
        return false;

    return (isLineEven(pos.y) ? pos.x <= Def::MAPX : pos.x < Def::MAPX) && pos.y <= Def::ARRAYMAPY;
}
