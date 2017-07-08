// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef DEFAULTGRAPHICSPOSCOMPONENT_HPP
#define DEFAULTGRAPHICSPOSCOMPONENT_HPP

#include <NDK/Component.hpp>

///
/// \struct DefaultGraphicsPosComponent
///
/// \brief Used by the factory for the sprite to fit
///        the default logic position
///

struct DefaultGraphicsPosComponent : public Ndk::Component<DefaultGraphicsPosComponent>
{
    DefaultGraphicsPosComponent(float x_ = 0.f, float y_ = 0.f) : x { x_ }, y { y_ } {}

    float x {};
    float y {};

    static Ndk::ComponentIndex componentIndex;
};

#endif // DEFAULTGRAPHICSPOSCOMPONENT_HPP