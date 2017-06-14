// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ELEMENTDATA_HPP
#define ELEMENTDATA_HPP

enum class Element
{
    Neutral,
    Air,
    Fire,
    Water,
    Earth
};

namespace std
{

template<>
struct hash<Element>
{
    using argument_type = Element;
    using result_type = std::size_t;

    ///
    /// \brief Specialisation of std to hash
    /// \return Result of the hash
    ///
    /// \param p Pair to hash
    ///

    result_type operator()(argument_type const& e) const
    {
        return static_cast<result_type>(e);
    }
};

}

#endif // ELEMENTDATA_HPP
