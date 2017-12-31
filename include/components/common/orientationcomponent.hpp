// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
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
    OrientationComponent(const Orientation& o = Orientation::Down) : dir { o } {}
    OrientationComponent(const OrientationComponent&) = default;
    Orientation dir;

    static Ndk::ComponentIndex componentIndex;
};

#endif // CDIRECTIONCOMPONENT_HPP
