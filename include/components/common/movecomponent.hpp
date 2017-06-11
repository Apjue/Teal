// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MOVECOMPONENT_HPP
#define MOVECOMPONENT_HPP

#include <NDK/Component.hpp>

struct MoveComponent : public Ndk::Component<MoveComponent>
{
    int diffX {}; // The position where we want to go
    int diffY {}; // Adds it to the Position's XY to get the tile
                  // 0 == nowhere
    /// \todo replace by absolute position

    bool playerInitiated {};

    static Ndk::ComponentIndex componentIndex;
};

#endif // MOVECOMPONENT_HPP
