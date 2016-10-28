// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RANDOM_HPP
#define RANDOM_HPP

///
/// \class RandomNumber
///
/// \brief Allows you to get a random number
///        from a generator of your choice
///
/// \example RandomNumber<std::mt19937> rng;
///          std::uniform_int_distribution<> uni(0, 5);
///          int random_number = uni(rng);
///

template<class Generator>
class RandomNumber
{
public:
    using result_type = typename Generator::result_type;

    result_type operator()();
    static result_type min();
    static result_type max();

private:
    static Generator& instance();
};

#include "random.inl"

#endif // RANDOM_HPP
