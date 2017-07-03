// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef HPGAINCOMPONENT_HPP
#define HPGAINCOMPONENT_HPP

#include <NDK/Component.hpp>
#include "def/typedef.hpp"

namespace Items
{

///
/// \struct HPGainComponent
///
/// \brief Adds diff HPs to the player
///        If diff == 0, the HP of the player becomes abs.
///

struct HPGainComponent : public Ndk::Component<HPGainComponent>
{
    HPGainComponent(const LuaArguments& args)
    {
        TealException(args.size() <= 2, "Too many arguments");

        if (args.size() > 0)
            diff = static_cast<int>(args[0].value.get<double>());

        if (args.size() > 1)
            abs = static_cast<unsigned>(args[1].value.get<double>());
    }

    int diff {}; // can be negative, e.g. for poison
    unsigned abs {};

    static Ndk::ComponentIndex componentIndex;
};

} // namespace Items

#endif // HPGAINCOMPONENT_HPP
