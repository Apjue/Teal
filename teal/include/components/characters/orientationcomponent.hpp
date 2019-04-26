// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CDIRECTIONCOMPONENT_HPP
#define CDIRECTIONCOMPONENT_HPP

#include <NDK/Component.hpp>
#include "global.hpp"

///
/// \struct OrientationComponent
///
/// \brief Contains the Orientation enum
///

struct OrientationComponent : public Ndk::Component<OrientationComponent>
{
    OrientationComponent(const Orientation& orientation_ = Orientation::Down) : orientation { orientation_ } {}
    OrientationComponent(const OrientationComponent&) = default;
    OrientationComponent(OrientationComponent&&) = default;

    Orientation orientation;

    static Ndk::ComponentIndex componentIndex;
};

#endif // CDIRECTIONCOMPONENT_HPP
