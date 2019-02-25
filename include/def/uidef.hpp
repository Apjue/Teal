// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UIDEF_HPP
#define UIDEF_HPP

#include "gamedef.hpp"

namespace Def
{

constexpr unsigned MapSizeX { (Def::ArrayMapX) * Def::TileSizeX };
constexpr unsigned MapSizeY { (Def::ArrayMapY + 1u) / 2 * Def::TileSizeY + TileSizeY / 2 };

constexpr unsigned ButtonsPaddingX { 10u };
constexpr unsigned ButtonsMarginY { 5u };

constexpr unsigned ButtonsSizeX { MapSizeX };
constexpr unsigned ButtonsSizeY { 60u };

constexpr unsigned WindowSizeX { MapSizeX };
constexpr unsigned WindowSizeY { MapSizeY + ButtonsMarginY * 2u + ButtonsSizeY };

}

#endif // UIDEF_HPP

