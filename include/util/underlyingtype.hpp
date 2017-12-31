// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UNDERLYINGTYPE_HPP
#define UNDERLYINGTYPE_HPP

#include <type_traits>

template<class T>
using EnumUnderlyingType = typename std::underlying_type<T>::type;

template<class T>
constexpr EnumUnderlyingType<T> toUnderlyingType(T enum_)
{
    return static_cast<EnumUnderlyingType<T>>(enum_);
}

#endif // UNDERLYINGTYPE_HPP
