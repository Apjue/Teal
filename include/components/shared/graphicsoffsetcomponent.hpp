// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_GRAPHICSOFFSETCOMPONENT_HPP
#define TEAL_GRAPHICSOFFSETCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Math/Vector2.hpp>

///
/// \struct GraphicsOffsetComponent
///
/// \brief Offset used to fit correctly in the [0;0] tile
/// \note Only used as a fallback when the AnimationComponent isn't available
///

struct GraphicsOffsetComponent : public Ndk::Component<GraphicsOffsetComponent>
{
    GraphicsOffsetComponent(const Nz::Vector2f& xy = {}) : offset { xy } {}

    Nz::Vector2f offset;

    static Ndk::ComponentIndex componentIndex;
};

#endif // TEAL_GRAPHICSOFFSETCOMPONENT_HPP
