// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RANDOMMOVEMENTCOMPONENT_HPP
#define RANDOMMOVEMENTCOMPONENT_HPP

#include <NDK/Component.hpp>

struct RandomMovementComponent : public Ndk::Component<RandomMovementComponent>
{
    RandomMovementComponent() = default;
    ~RandomMovementComponent() = default;

    RandomMovementComponent(float movInt, unsigned nbTiles_)
        : movingInterval { movInt }, range { nbTiles_ } {}

    float movingInterval {};
    unsigned range {}; // Number of tiles to move each movement
    float currentInterval {};

    static Ndk::ComponentIndex componentIndex;
};

#endif // RANDOMMOVEMENTCOMPONENT_HPP
