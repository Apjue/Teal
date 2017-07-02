// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATICMAX_HPP
#define STATICMAX_HPP

#include <cstddef>

template <std::size_t arg1, std::size_t... others>
struct StaticMax;

template <std::size_t arg>
struct StaticMax<arg>
{
    static const std::size_t value = arg;
};

template <std::size_t arg1, std::size_t arg2, std::size_t... others>
struct StaticMax<arg1, arg2, others...>
{
    static const std::size_t value = arg1 >= arg2 ? StaticMax<arg1, others...>::value : StaticMax<arg2, others...>::value;
};

#endif // STATICMAX_HPP
