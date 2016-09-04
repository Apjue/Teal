// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RANDOMMOVEMENTCOMPONENT_HPP
#define RANDOMMOVEMENTCOMPONENT_HPP

#include <NDK/Component.hpp>

struct RandomMovementComponent : public Ndk::Component<RandomMovementComponent>
{
    RandomMovementComponent() = default;
    ~RandomMovementComponent() = default;

    float movingInterval {};
    unsigned nbTiles {}; // Number of tiles to move each movement

    static Ndk::ComponentIndex componentIndex;
};

#endif // RANDOMMOVEMENTCOMPONENT_HPP
