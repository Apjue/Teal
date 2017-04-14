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
#include "def/gamedef.hpp"

struct PathComponent : public Ndk::Component<PathComponent>
{
    using PathPool = std::queue<std::pair<DirectionFlags, bool>>;

    PathPool path;
    unsigned totalSize {};
    AbsTile wantedPos { Def::LMAPX + 1, Def::LMAPY + 1 };

    static Ndk::ComponentIndex componentIndex;
};

#endif // PATHCOMPONENT_HPP
