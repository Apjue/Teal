// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_MAPOBJECTCOMPONENT_HPP
#define TEAL_MAPOBJECTCOMPONENT_HPP

#include <NDK/Component.hpp>

struct MapObjectComponent : public Ndk::Component<MapObjectComponent>
{
    static Ndk::ComponentIndex componentIndex;
};

#endif // TEAL_MAPOBJECTCOMPONENT_HPP
