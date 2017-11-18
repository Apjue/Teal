// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <Nazara/Core/String.hpp>
#include <array>
#include <memory>
#include <vector>
#include "util/variant.hpp"
#include "gamedef.hpp"

struct TileData;
using TileArray = std::array<TileData, Def::TileArraySize>;

using LuaBasicArgument = Variant<Nz::String, double, bool>;

struct LuaTableArgument
{
    std::vector<LuaBasicArgument> vars;
    std::vector<std::shared_ptr<LuaTableArgument>> tables;
};

using LuaArgument = LuaTableArgument;
using LuaArguments = LuaTableArgument;

#endif // TYPEDEF_HPP
