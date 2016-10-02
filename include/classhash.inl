// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

namespace std
{

template<class T>
struct hash<Nz::Vector2<T>>
{
    using argument_type = Nz::Vector2<T>;
    using result_type = std::size_t;

    ///
    /// \brief Specialisation of std to hash
    /// \return Result of the hash
    ///
    /// \param v Vector2 to hash
    ///

    result_type operator()(argument_type const& v) const
    {
        result_type seed {};
        Nz::HashCombine(seed, v.x);
        Nz::HashCombine(seed, v.y);
        return seed;
    }
};

template<class A, class B>
struct hash<std::pair<A, B>>
{
    using argument_type = std::pair<A, B>;
    using result_type = std::size_t;

    ///
    /// \brief Specialisation of std to hash
    /// \return Result of the hash
    ///
    /// \param p Pair to hash
    ///

    result_type operator()(argument_type const& p) const
    {
        result_type seed {};
        Nz::HashCombine(seed, p.first);
        Nz::HashCombine(seed, p.second);
        return seed;
    }
};

}
