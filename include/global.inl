// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

DiffTile OrientToDiff(Orientation o)
{
    return DirToXY(OrienToDir(o));
}

bool isPositionValid(AbsTile pos)
{
    return pos.x % 2 == pos.y % 2;
}
