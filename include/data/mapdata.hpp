// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPDATA_HPP
#define MAPDATA_HPP

#include <Nazara/Graphics/Material.hpp>
#include "def/gamedef.hpp"

struct MapData
{
    OLDTILEARRAY map;
    OLDTILEARRAY obs;
    Nz::String tileset;
};

#endif // MAPDATA_HPP