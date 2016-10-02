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
        // Boost algorithm
        // http://www.boost.org/doc/libs/1_37_0/doc/html/hash/reference.html#boost.hash_value_id2443661

        result_type seed = std::hash<T> {}(v.x);
        seed ^= std::hash<T> {}(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    }
};

}
