// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPPOSITIONCOMPONENT_HPP
#define MAPPOSITIONCOMPONENT_HPP

#include <NDK/Component.hpp>

struct MapPositionComponent : public Ndk::Component<MapPositionComponent>
{
    MapPositionComponent(int x_ = 0, int y_ = 0)
        : x { x_ }, y { y_ } {}

    int x {};
    int y {};

    static Ndk::ComponentIndex componentIndex;
};

#endif // MAPPOSITIONCOMPONENT_HPP
