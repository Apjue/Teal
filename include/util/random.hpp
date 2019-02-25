// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

#ifdef TEAL_RANDOM_DEVICE_BROKEN

#include <ctime>
#include "util/util.hpp"

#endif

///
/// \class RandomNumber
///
/// \brief Allows you to get a random number
///        from a generator of your choice
///
/// \example RandomNumber<std::mt19937> rng;
///          std::uniform_int_distribution<> uni(0, 5);
///          int number = uni(rng);
///

template<class Generator>
class RandomNumber
{
public:
    using result_type = typename Generator::result_type;

    result_type operator()();
    static constexpr result_type min();
    static constexpr result_type max();

    static Generator generator;
};

template<class Generator>
Generator RandomNumber<Generator>::generator
(
    #ifdef TEAL_RANDOM_DEVICE_BROKEN
        unsigned(std::time(nullptr))
    #else
        std::random_device {}()
    #endif
);

#include "random.inl"

#endif // RANDOM_HPP
