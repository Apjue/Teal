// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <Nazara/Core/String.hpp>
#include <array>
#include <vector>
#include "util/variant.hpp"
#include "gamedef.hpp"

struct TileData;
using TILEARRAY = std::array<TileData, Def::TILEARRAYSIZE>;

struct LuaArgument
{
    Variant<Nz::String, double, bool, LuaArgument*> value;
};

using LuaArguments = std::vector<LuaArgument>;

#endif // TYPEDEF_HPP
