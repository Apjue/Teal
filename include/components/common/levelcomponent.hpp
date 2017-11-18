// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LEVELCOMPONENT_HPP
#define LEVELCOMPONENT_HPP

#include <NDK/Component.hpp>

struct LevelComponent : public Ndk::Component<LevelComponent>
{
    LevelComponent(unsigned lvl = 1) : level { lvl } {}

    unsigned level {};

    static Ndk::ComponentIndex componentIndex;
};

#endif // LEVELCOMPONENT_HPP
