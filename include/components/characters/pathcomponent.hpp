// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef PATHCOMPONENT_HPP
#define PATHCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <vector>
#include <utility>
#include "global.hpp"
#include "def/gamedef.hpp"

struct PathComponent : public Ndk::Component<PathComponent>
{
    using PathPool = std::vector<DirectionFlags>;
    PathPool path;

    static Ndk::ComponentIndex componentIndex;
};

#endif // PATHCOMPONENT_HPP
