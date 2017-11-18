// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAMEDEF_HPP
#define GAMEDEF_HPP

#include <array>
#include <utility>
#include <limits>

namespace Def
{

constexpr unsigned MapX           { 7u };
constexpr unsigned MapY           { 14u };

constexpr unsigned ArrayMapX      { MapX + 1u };
constexpr unsigned ArrayMapY      { MapY + 1u };


constexpr unsigned TileArraySize  { ArrayMapX * ArrayMapY };
constexpr unsigned TileSizeX      { 64u }; // For logic
constexpr unsigned TileSizeY      { 32u };


constexpr   int    MaxPosInTile   { 4 }; // Must be dividable by TileSizeX & TileSizeY
constexpr unsigned MaxGXPosInTile { (TileSizeX / 2) / MaxPosInTile };
constexpr unsigned MaxGYPosInTile { (TileSizeY / 2) / MaxPosInTile };

constexpr  float   MaxSystemUPS   { 10 }; // Updates per second
constexpr   int    MaxDirs        { 4  }; // Max directions: Up, down, left, right
constexpr std::pair<unsigned, unsigned> StandStillPos { std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::max() }; // with MoveComponent


constexpr std::array<int,   8> MapDistanceEvenX   { 0,    0,   1,   0,   0,   -1,  -1,  -1   }; // See Orientation
constexpr std::array<int,   8> MapDistanceEvenY   { 2,    1,   0,  -1,  -2,   -1,   0,   1   };

constexpr std::array<int,   8> MapDistanceUnevenX { 0,    1,   1,   1,   0,    0,  -1,   0   };
constexpr std::array<int,   8> MapDistanceUnevenY { 2,    1,   0,  -1,  -2,   -1,   0,   1   };

constexpr std::array<float, 8> MapDistanceCost    { 1.5f, 1.f, 2.f, 1.f, 1.5f, 1.f, 2.f, 1.f };

constexpr unsigned DefaultFightTimeLimit { 60'000 }; // In Miliseconds

}

#endif // GAMEDEF_HPP
