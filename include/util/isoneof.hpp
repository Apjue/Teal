// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ISONEOF_HPP
#define ISONEOF_HPP

#include <type_traits>

template <typename...>
struct IsOneOf
{
    static constexpr bool value = false;
};

template <typename T, typename S, typename... Ts>
struct IsOneOf<T, S, Ts...>
{
    static constexpr bool value = std::is_same<T, S>::value || IsOneOf<T, Ts...>::value;
};

#endif // ISONEOF_HPP
