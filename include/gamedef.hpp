// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAMEDEF_HPP
#define GAMEDEF_HPP

#include <array>
#include "tileinfos.hpp"

namespace Def
{

constexpr unsigned MAPX           { 15u }; //For arrays.
constexpr unsigned MAPY           { 8u };

constexpr unsigned TILEARRAYSIZE  { MAPX * MAPY };

constexpr unsigned LMAPX          { 14u }; //For logic
constexpr unsigned LMAPY          { 14u };

constexpr unsigned MAPXSIZE       { 512u };
constexpr unsigned MAPYSIZE       { 256u };

constexpr unsigned BUTTONSXSIZE   { MAPXSIZE };
constexpr unsigned BUTTONSYSIZE   { 90u };

constexpr unsigned WINXSIZE       { MAPXSIZE };
constexpr unsigned WINYSIZE       { MAPYSIZE + BUTTONSYSIZE };

constexpr unsigned TILEXSIZE      { 64u };
constexpr unsigned TILEYSIZE      { 32u };

constexpr unsigned TILEGXSIZE     { TILEXSIZE / 2 };
constexpr unsigned TILEGYSIZE     { TILEYSIZE / 2 };

constexpr unsigned MAXPOSINTILE   { 4u };
constexpr unsigned MAXGXPOSINTILE { TILEGXSIZE / MAXPOSINTILE };
constexpr unsigned MAXGYPOSINTILE { TILEGYSIZE / MAXPOSINTILE };

constexpr float    MAXFPS         { 10 }; // Updates per second

constexpr int      MAXDIR         { 4 }; // Max directions: Up, down, left, right

}

using OLDTILEARRAY = std::array<unsigned, Def::TILEARRAYSIZE>;
using TILEARRAY = std::array<TileInfos, Def::TILEARRAYSIZE>;

#endif // GAMEDEF_HPP