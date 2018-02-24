// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CLONECOMPONENT_HPP
#define CLONECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/String.hpp>

struct CloneComponent : public Ndk::Component<CloneComponent>
{
    Nz::String codename;

    static Ndk::ComponentIndex componentIndex;
};

#endif // CLONECOMPONENT_HPP
