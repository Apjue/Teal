// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

Orientation stringToOrientation(Nz::String string)
{

}


DiffTile OrientToDiff(Orientation o)
{
    return DirToXY(OrientToDir(o));
}

bool isPositionValid(AbsTile pos)
{
    return pos.x % 2 == pos.y % 2;
}
