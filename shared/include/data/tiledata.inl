// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "tiledata.hpp"

inline TileFlag stringToTileFlag(Nz::String string)
{
    string = string.ToLower();

    if (string == "none")
        return TileFlag::NoFlag;


    if (string == "inv")
        return TileFlag::Invisible;


    if (string == "viewobs")
        return TileFlag::ViewObstacle;

    if (string == "blockobs")
        return TileFlag::BlockObstacle;


    if (string == "red")
        return TileFlag::RedSpawn;

    if (string == "blue")
        return TileFlag::BlueSpawn;

    throw std::runtime_error { "Invalid Tile Flag" };
}

inline Nz::String tileFlagToString(TileFlag flag)
{
    switch (flag)
    {
        case TileFlag::NoFlag:
            return "none";


        case TileFlag::Invisible:
            return "inv";


        case TileFlag::ViewObstacle:
            return "viewobs";

        case TileFlag::BlockObstacle:
            return "blockobs";


        case TileFlag::RedSpawn:
            return "red";

        case TileFlag::BlueSpawn:
            return "blue";
    }

    throw std::runtime_error { "Invalid Tile Flag" };
}

inline bool TileData::isVisible() const
{
    return flag != TileFlag::Invisible;
}


inline bool TileData::isObstacle() const
{
    return isViewObstacle() || isBlockObstacle();
}

inline bool TileData::isViewObstacle() const
{
    return flag == TileFlag::ViewObstacle;
}

inline bool TileData::isBlockObstacle() const
{
    return flag == TileFlag::BlockObstacle;
}

inline bool TileData::isWalkable() const
{
    return !isObstacle() && !occupied;
}


inline bool TileData::isSpawn() const
{
    return isRedSpawn() || isBlueSpawn();
}

inline bool TileData::isRedSpawn() const
{
    return flag == TileFlag::RedSpawn;
}

inline bool TileData::isBlueSpawn() const
{
    return flag == TileFlag::BlueSpawn;
}
