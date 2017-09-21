// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ELEMENTDATA_HPP
#define ELEMENTDATA_HPP

#include "util/underlyingtype.hpp"

enum class Element
{
    Neutral,
    Air,
    Fire,
    Water,
    Earth,

    Max = Earth
};

inline Element& operator++(Element& e)
{
    EnumUnderlyingType<Element> i = toUnderlyingType<Element>(e);
    e = static_cast<Element>(++i);

    return e;
}

inline Element stringToElement(Nz::String string)
{
    string = string.ToLower();
        
    if (string == "air")
        return Element::Air;

    if (string == "fire")
        return Element::Fire;

    if (string == "water")
        return Element::Water;

    if (string == "earth")
        return Element::Earth;

    return Element::Neutral;
}

inline Nz::String elementToString(Element e)
{
    switch (e)
    {
        case Element::Neutral:
            return "neutral";

        case Element::Air:
            return "air";

        case Element::Fire:
            return "fire";

        case Element::Water:
            return "water";

        case Element::Earth:
            return "earth";

        default:
            return "";
    }
}

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
