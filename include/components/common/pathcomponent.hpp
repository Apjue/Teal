// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef PATHCOMPONENT_HPP
#define PATHCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <queue>
#include <utility>
#include "global.hpp"

struct PathComponent : public Ndk::Component<PathComponent>
{
    std::queue<std::pair<DirectionFlags, bool>> path;
    //To add: path.push(...);
    //To read: path.front(); && path.pop();

    static Ndk::ComponentIndex componentIndex;
};

#endif // PATHCOMPONENT_HPP
