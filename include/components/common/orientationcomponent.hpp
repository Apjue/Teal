// Copyright (C) 2018 Samy Bensaid
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
/// \todo Change name to Orientation ?
///

struct OrientationComponent : public Ndk::Component<OrientationComponent>
{
    OrientationComponent(const Orientation& orientation = Orientation::Down) : orientation { orientation } {}
    OrientationComponent(const OrientationComponent&) = default;
    Orientation orientation;

    static Ndk::ComponentIndex componentIndex;
};

#endif // CDIRECTIONCOMPONENT_HPP
