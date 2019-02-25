// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ELEMENTDATA_HPP
#define ELEMENTDATA_HPP

#include <Nazara/Core/String.hpp>
#include <stdexcept>
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

    if (string == "neutral")
        return Element::Neutral;
        
    if (string == "air")
        return Element::Air;

    if (string == "fire")
        return Element::Fire;

    if (string == "water")
        return Element::Water;

    if (string == "earth")
        return Element::Earth;

    throw std::runtime_error { "Invalid element !" };
}

inline const char* elementToString(Element e)
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
            throw std::runtime_error { "Invalid element !" };
    }
}

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, Element* element, TypeTag<Element>)
{
    state.CheckType(index, Nz::LuaType_String);
    *element = stringToElement(state.CheckString(index));

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, Element&& element, TypeTag<Element>)
{
    state.PushString(elementToString(element));
    return 1;
}

} // namespace Nz

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
