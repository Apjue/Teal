// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef FIGHTCOMPONENT_HPP
#define FIGHTCOMPONENT_HPP

#include <NDK/Component.hpp>

struct FightComponent : public Ndk::Component<FightComponent>
{
    bool isFighting { false };

    static Ndk::ComponentIndex componentIndex;
};

#endif // FIGHTCOMPONENT_HPP