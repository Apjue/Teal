// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPPOSITIONCOMPONENT_HPP
#define MAPPOSITIONCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector2.hpp>

struct MapPositionComponent : public Ndk::Component<MapPositionComponent>
{
    MapPositionComponent(const Nz::Vector2i& pos = {}) : xy { pos } {}

    Nz::Vector2i xy;

    static Ndk::ComponentIndex componentIndex;
};

#endif // MAPPOSITIONCOMPONENT_HPP
