// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MOVECOMPONENT_HPP
#define MOVECOMPONENT_HPP

#include <NDK/Component.hpp>
#include "util/nzstlcompatibility.hpp"
#include "global.hpp"

struct MoveComponent : public Ndk::Component<MoveComponent>
{
    AbsTile tile { toVector(Def::StandStillPos) }; // The absolute position where we want to go
                  // Def::StandStillPos == nowhere

    bool playerInitiated {}; // If player clicks to the same location and if this is set to true, AISystem ignore the request
                             // Else, AISystem recompute a path (maybe something moved and is now blocking the path ?)

    static Ndk::ComponentIndex componentIndex;
};

#endif // MOVECOMPONENT_HPP
