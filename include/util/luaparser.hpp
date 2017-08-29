// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LUAPARSER_HPP
#define LUAPARSER_HPP

#include <Nazara/Lua.hpp>
#include <Nazara/Core/String.hpp>
#include "def/typedef.hpp"

inline LuaArguments parseLua(Nz::LuaInstance& lua);
extern void parseLua(Nz::LuaInstance& lua, LuaArguments& table);

inline bool hasField(Nz::LuaInstance& lua, const Nz::String& fieldName, int tableIndex = -1);

#include "luaparser.inl"

#endif // LUAPARSER_HPP
