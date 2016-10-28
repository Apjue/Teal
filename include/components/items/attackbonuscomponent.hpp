// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ATTACKBONUSCOMPONENT_HPP
#define ATTACKBONUSCOMPONENT_HPP

#include <NDK/Component.hpp>

namespace Items
{

struct AttackBonusComponent : public Ndk::Component<AttackBonusComponent>
{
    int air {};
    int fire {};
    int water {};
    int earth {};
    int neutral {};

    static Ndk::ComponentIndex componentIndex;
};

}

#endif // ATTACKBONUSCOMPONENT_HPP
