// Copyright (C) 2018 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef NZSTLCOMPATIBILITY_HPP
#define NZSTLCOMPATIBILITY_HPP

// Nz::Vector2 and std::pair

#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Lua/LuaState.hpp>
#include <NDK/LuaAPI.hpp>
#include <unordered_map>
#include <type_traits>
#include <utility>
#include <vector>
#include <map>

template<class T>
inline std::pair<T, T> toPair(const Nz::Vector2<T>& v);

template<class T>
inline Nz::Vector2<T> toVector2(const std::pair<T, T>& v);

namespace Nz
{

template<class T1, class T2>
inline unsigned LuaImplQueryArg(const LuaState& state, int index, std::pair<T1, T2>* pair, TypeTag<std::pair<T1, T2>>);


template<class T1, class T2>
inline unsigned LuaImplQueryArg(const LuaState& state, int index, std::unordered_map<T1, T2>* map, TypeTag<std::unordered_map<T1, T2>>);

template<class T1, class T2>
inline unsigned LuaImplQueryArg(const LuaState& state, int index, std::map<T1, T2>* map, TypeTag<std::map<T1, T2>>);

} // namespace Nz

#include "nzstlcompatibility.inl"

#endif // NZSTLCOMPATIBILITY_HPP
