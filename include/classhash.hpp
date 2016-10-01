// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CLASSHASH_HPP
#define CLASSHASH_HPP

#include <functional>
#include <Nazara/Math/Vector2.hpp>

namespace std
{

template<>
struct hash<Nz::Vector2i>
{
    using argument_type = Nz::Vector2i;
    using result_type = std::size_t;

    result_type operator()(argument_type const& s) const
    {
        result_type const h1( std::hash<int>{} (s.x) );
        result_type const h2( std::hash<int>{} (s.y) );

        return h1 ^ (h2 << 1);
    }
};

}

#endif // CLASSHASH_HPP
