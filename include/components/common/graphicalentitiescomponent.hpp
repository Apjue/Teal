// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GRAPHICALENTITIESCOMPONENT_HPP
#define GRAPHICALENTITIESCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/EntityList.hpp>

struct GraphicalEntitiesComponent : public Ndk::Component<GraphicalEntitiesComponent>
{
    Ndk::EntityList entities;

    static Ndk::ComponentIndex componentIndex;
};

#endif // GRAPHICALENTITIESCOMPONENT_HPP
