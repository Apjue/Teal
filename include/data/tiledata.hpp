// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TILEDATA_HPP
#define TILEDATA_HPP

#include <Nazara/Core/String.hpp>

struct TileData
{
    Nz::String textureId;
    unsigned obstacle {}; // 0 = can pass, 1 = can't pass but can view through (in fight), 2 = can't pass and can't view through
    bool visible { true };
    bool occupied {}; // Occupied by a movable (or not) NPC. Not available in fight
};

#endif // TILEDATA_HPP
