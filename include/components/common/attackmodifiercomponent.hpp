// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ATTACKBONUSCOMPONENT_HPP
#define ATTACKBONUSCOMPONENT_HPP

#include <NDK/Component.hpp>

struct AttackModifierComponent : public Ndk::Component<AttackModifierComponent>
{
    int neutral {};
    int air {};
    int fire {};
    int water {};
    int earth {};

    static Ndk::ComponentIndex componentIndex;
};

#endif // ATTACKBONUSCOMPONENT_HPP
