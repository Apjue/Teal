// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef INDEXOF_HPP
#define INDEXOF_HPP

#include <type_traits>

template <typename...>
struct IndexOf;

// Found
template <class T, class... Rest>
struct IndexOf<T, T, Rest...> : std::integral_constant<std::size_t, 0u> {};

// Still searching
template <class T, class Other, class... Rest>
struct IndexOf<T, Other, Rest...> : std::integral_constant<std::size_t, 1 + IndexOf<T, Rest...>::value> {};

#endif // INDEXOF_HPP
