// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef DEFAULTGRAPHICSPOSCOMPONENT_HPP
#define DEFAULTGRAPHICSPOSCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector2.hpp>

///
/// \struct DefaultGraphicsPosComponent
///
/// \brief Offset used to fit correctly in the [1;1] tile
/// \note Only used as a fallback when the AnimationComponent isn't available
///

struct DefaultGraphicsPosComponent : public Ndk::Component<DefaultGraphicsPosComponent>
{
    DefaultGraphicsPosComponent(const Nz::Vector2f& xy = {}) : offset { xy } {}

    Nz::Vector2f offset;

    static Ndk::ComponentIndex componentIndex;
};

#endif // DEFAULTGRAPHICSPOSCOMPONENT_HPP
