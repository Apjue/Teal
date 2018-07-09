// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAMEDEF_HPP
#define GAMEDEF_HPP

#include <array>
#include <utility>
#include <limits>

namespace Def
{

constexpr unsigned MapX           { 7u  }; // Map size
constexpr unsigned MapY           { 14u };
static_assert((MapX % 2) == 1 && (MapY % 2) == 0, "Invalid map size");

constexpr unsigned ArrayMapX      { MapX + 1u };
constexpr unsigned ArrayMapY      { MapY + 1u };


constexpr unsigned TileArraySize    { ArrayMapX * ArrayMapY };
constexpr unsigned TileSizeX        { 64u }; // For logic
constexpr unsigned TileSizeY        { 32u };
constexpr unsigned TileVertexNumber { 4u };


constexpr   int    MaxPosInTile { 4u }; // Must be dividable by TileSizeX & TileSizeY
static_assert((TileSizeX % MaxPosInTile) == 0 && (TileSizeY % MaxPosInTile) == 0, "MaxPosInTile must be dividable by TileSizeX & Y");

constexpr unsigned MaxGraphicsXPosInTile { (TileSizeX / 2u) / MaxPosInTile };
constexpr unsigned MaxGraphicsYPosInTile { (TileSizeY / 2u) / MaxPosInTile };


constexpr  float   MaxSystemUPS   { 10.f }; // Updates per second
constexpr   int    MaxDirs        { 4u   }; // Max directions: Up, down, left, right
constexpr std::pair<unsigned, unsigned> StandStillPos { std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::max() }; // with MoveComponent


constexpr std::array<int,   8> MapDistanceEvenX   { 0,    0,   1,   0,   0,   -1,  -1,  -1   }; // See Orientation
constexpr std::array<int,   8> MapDistanceEvenY   { 2,    1,   0,  -1,  -2,   -1,   0,   1   };

constexpr std::array<int,   8> MapDistanceUnevenX { 0,    1,   1,   1,   0,    0,  -1,   0   };
constexpr std::array<int,   8> MapDistanceUnevenY { 2,    1,   0,  -1,  -2,   -1,   0,   1   };

constexpr std::array<float, 8> MapDistanceCost    { 1.5f, 1.f, 2.f, 1.f, 1.5f, 1.f, 2.f, 1.f };


constexpr unsigned   DefaultFightTimeLimit { 60'000u }; // In Miliseconds
constexpr Nz::UInt32 LuaAITimeLimit { 1000u };

constexpr std::size_t PathRunningAfter { 2 };

}

#endif // GAMEDEF_HPP
