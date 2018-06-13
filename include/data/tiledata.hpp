// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TILEDATA_HPP
#define TILEDATA_HPP

#include <Nazara/Core/String.hpp>

struct TileData
{
    bool occupied {}; // Occupied by a NPC or monster

    Nz::String textureId;
    Nz::String fightTextureId;

    ///
    /// \brief Flags a tile can have
    ///        Possible flags:
    ///        - invisible
    ///        - viewobstacle
    ///        - blockobstacle
    ///        - redspawn
    ///        - bluespawn
    ///

    Nz::String flags;

    inline void addFlag(const Nz::String& flag)
    {
        if (!flags.IsEmpty())
            flags.Append(" ; ");

        flags.Append(flag);
    }

    inline bool isVisible()  const { return !flags.Contains("invisible"); }
    inline bool isObstacle() const { return flags.Contains("obstacle"); }
    inline bool isViewObstacle() const { return flags.Contains("viewobstacle"); }
    inline bool isBlockObstacle() const { return flags.Contains("blockobstacle"); }
    inline bool isWalkable() const { return !isObstacle() && !occupied;   }
    inline bool isSpawn()    const { return flags.Contains("spawn");      }
};

#endif // TILEDATA_HPP
