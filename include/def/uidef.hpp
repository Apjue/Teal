// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UIDEF_HPP
#define UIDEF_HPP

#include "gamedef.hpp"

namespace Def
{

constexpr unsigned MapSizeX { (Def::ArrayMapX) * Def::TileSizeX };
constexpr unsigned MapSizeY { (Def::ArrayMapY + 1u) / 2 * Def::TileSizeY + TileSizeY / 2 };

constexpr unsigned ButtonsMarginX { 10u };
constexpr unsigned ButtonsMarginY { 5u };

constexpr unsigned ButtonsSizeX { MapSizeX };
constexpr unsigned ButtonsSizeY { 90u };

constexpr unsigned WindowSizeX { MapSizeX };
constexpr unsigned WindowSizeY { MapSizeY + ButtonsMarginY + ButtonsSizeY };

}

#endif // UIDEF_HPP

