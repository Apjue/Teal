// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include <NDK/Component.hpp>

struct PositionComponent : public Ndk::Component<PositionComponent>
{
    PositionComponent(unsigned x_ = 0, unsigned y_ = 0) : x { x_ }, y { y_ } {}

    unsigned x {}; //The actual position
    unsigned y {}; //(absolute)

                   //num:
                   //X: +32px == +1
                   //Y: +16px == +1

    int inX {}; //Position in tile from x/y.
    int inY {}; //difference.

    bool moving { false };

    static Ndk::ComponentIndex componentIndex;
};

#endif // POSITIONCOMPONENT_HPP