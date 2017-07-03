// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EDIBLECOMPONENT_HPP
#define EDIBLECOMPONENT_HPP

#include <NDK/Component.hpp>
#include "def/typedef.hpp"

namespace Items
{

struct EdibleComponent : public Ndk::Component<EdibleComponent>
{
    EdibleComponent() = default;
    EdibleComponent(const LuaArguments&) {}

    static Ndk::ComponentIndex componentIndex;
};

}

#endif // EDIBLECOMPONENT_HPP
