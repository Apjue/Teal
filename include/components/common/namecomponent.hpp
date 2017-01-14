// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef NAMECOMPONENT_HPP
#define NAMECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/String.hpp>

struct NameComponent : public Ndk::Component<NameComponent>
{
    NameComponent(const Nz::String& name_ = "") : name(name_) {}

    Nz::String name {};

    static Ndk::ComponentIndex componentIndex;
};

#endif // NAMECOMPONENT_HPP
