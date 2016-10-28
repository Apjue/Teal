// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MOVETOCOMPONENT_HPP
#define MOVETOCOMPONENT_HPP

#include <NDK/Component.hpp>

struct MoveToComponent : public Ndk::Component<MoveToComponent>
{
    int diffX {}; //The position where we want to go
    int diffY {}; //Adds it to the Position's XY to get the tile
                  //0 == nowhere

    static Ndk::ComponentIndex componentIndex;
};

#endif // MOVETOCOMPONENT_HPP