// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include <NDK/Component.hpp>
#include "global.hpp"

struct PositionComponent : public Ndk::Component<PositionComponent>
{
    PositionComponent() = default;
    PositionComponent(const AbsTile& pos) : xy { pos } {}

    AbsTile xy {}; // The actual position (absolute)

    DirectionFlags direction; // replaces inXY
    unsigned advancement {};

    bool moving { false };

    static Ndk::ComponentIndex componentIndex;
};

#endif // POSITIONCOMPONENT_HPP