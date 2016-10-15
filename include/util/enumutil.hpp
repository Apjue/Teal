// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ENUMUTIL_HPP
#define ENUMUTIL_HPP

#include <type_traits>
#include "def/gamedef.hpp"

template<class T>
using EnumUnderlyingType = typename std::underlying_type<T>::type;

template<class T>
inline constexpr T operator&(T a, T b);

template<class T>
inline constexpr T operator|(T a, T b);

template<class T>
inline void operator|=(T& a, T b);

template<class T>
inline void operator&=(T& a, T b);

template<class T>
inline constexpr T EnumReverse(T a, unsigned flags);

#include "enumutil.inl"

#endif // ENUMUTIL_HPP
