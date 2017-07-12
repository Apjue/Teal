// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <ctime>

#ifdef TEAL_USE_STDRAND

#include <cstdlib>

std::srand(static_cast<unsigned>(std::time(nullptr)));

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
    static result_type min();
    static result_type max();

    static Generator generator;
};

template<class Generator>
Generator RandomNumber<Generator>::generator
{
#ifdef TEAL_USE_STDRAND
    std::rand()
#else
    static_cast<unsigned>(std::time(nullptr))
#endif
};

#include "random.inl"

#endif // RANDOM_HPP
