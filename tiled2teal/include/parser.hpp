// Copyright (C) 2017 Samy Bensaid
// This file is part of the Tiled2Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LUA_HPP
#define LUA_HPP

#include <Nazara/Core/String.hpp>
#include <vector>

void initTilesetCore();
void convertTiledMap(const Nz::String& tex, const Nz::String& ftex, const Nz::String& inv, const Nz::String& obs, const Nz::String& spawn, 
                     const Nz::String& out, const Nz::String& pos);
std::vector<int> parseCsv(const Nz::String& file);

#endif // LUA_HPP
