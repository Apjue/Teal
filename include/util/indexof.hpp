// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef INDEXOF_HPP
#define INDEXOF_HPP

#include <type_traits>

template <class...>
struct IndexOf;

// Found
template <class T, class... Rest>
struct IndexOf<T, T, Rest...> : std::integral_constant<std::size_t, 1u> {};

// Still searching
template <class T, class Other, class... Rest>
struct IndexOf<T, Other, Rest...> : std::integral_constant<std::size_t, 1 + IndexOf<T, Rest...>::value> {};

#endif // INDEXOF_HPP
