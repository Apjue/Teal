// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CLASSHASH_HPP
#define CLASSHASH_HPP

#include <functional>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Core/String.hpp>

namespace std
{

template<class T>
struct hash<Nz::Vector2<T>>
{
    using argument_type = Nz::Vector2<T>;
    using result_type = std::size_t;

    result_type operator()(argument_type const& s) const
    {
//         result_type const h1 { std::hash<T>{} (s.x) };
//         result_type const h2 { std::hash<T>{} (s.y) };
// 
//         return h1 ^ h2;

        result_type seed = std::hash(s.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed ^ (std::hash(s.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    }
};

template<>
struct hash<Nz::String>
{
    using argument_type = Nz::String;
    using result_type = std::size_t;

    result_type operator()(argument_type const& s) const
    {
        return std::hash<std::string>{} (s);
    }
};

}

#endif // CLASSHASH_HPP
