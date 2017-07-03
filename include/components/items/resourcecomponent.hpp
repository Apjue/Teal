// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RESOURCECOMPONENT_HPP
#define RESOURCECOMPONENT_HPP

#include <NDK/Component.hpp>
#include "def/typedef.hpp"

namespace Items
{

struct ResourceComponent : public Ndk::Component<ResourceComponent>
{
    ResourceComponent(const LuaArguments&) {}

    static Ndk::ComponentIndex componentIndex;
};

} // namespace Items

#endif // RESOURCECOMPONENT_HPP