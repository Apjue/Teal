// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAMEDEF_HPP
#define GAMEDEF_HPP

#include <array>
#include <utility>

namespace Def
{

constexpr unsigned REALMAPX       { 8u  }; // see realnum.png
constexpr unsigned REALMAPY       { 8u  };

constexpr unsigned LONGMAPX       { 15u }; // see longnum.png
constexpr unsigned LONGMAPY       { 16u };

constexpr unsigned MAPX           { LONGMAPX }; // For arrays
constexpr unsigned MAPY           { REALMAPY };

constexpr unsigned LMAPX          { LONGMAPX - 1u }; // see logicnum.png
constexpr unsigned LMAPY          { LONGMAPY - 1u };


constexpr unsigned TILEARRAYSIZE  { MAPX * MAPY };
constexpr  float   TILESETSIZE    { 256 };

constexpr unsigned TILEXSIZE      { 64u }; // For logic
constexpr unsigned TILEYSIZE      { 32u };

constexpr unsigned TILEGXSIZE     { TILEXSIZE / 2 };
constexpr unsigned TILEGYSIZE     { TILEYSIZE / 2 };


constexpr   int    MAXPOSINTILE   { 4 };
constexpr unsigned MAXGXPOSINTILE { TILEGXSIZE / MAXPOSINTILE };
constexpr unsigned MAXGYPOSINTILE { TILEGYSIZE / MAXPOSINTILE };

constexpr  float   MAXSYSTEMUPS   { 10 }; // Updates per second
constexpr   int    MAXDIR         { 4 }; // Max directions: Up, down, left, right

constexpr  auto    DEFAULTMAPTILESET { ":/game/tileset" };

constexpr std::array<int,   8> MAP_DISTANCE_X    { 0,   2,   0 , -2,   1,   -1,    1,   -1    };
constexpr std::array<int,   8> MAP_DISTANCE_Y    { 2,   0,  -2,   0,   1,   -1,   -1,    1    };
constexpr std::array<float, 8> MAP_DISTANCE_COST { 2.f, 2.f, 2.f, 2.f, 1.5f, 1.5f, 1.5f, 1.5f };

constexpr std::pair<unsigned, unsigned> NOMOVEPOS { 20u, 20u }; // with MoveComponent

}

#endif // GAMEDEF_HPP
