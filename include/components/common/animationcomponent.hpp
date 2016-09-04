// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Math/Rect.hpp>

struct AnimationComponent : public Ndk::Component<AnimationComponent>
{
    enum AnimationState
    {
        Undefined,
        Moving
    };

    ///
    /// \fn AnimationComponent
    ///
    /// \param s Size of the picture
    /// \param mf Max Frames of the animation
    /// \param df Default Frame of the animation
    ///

    AnimationComponent(const Nz::Rectui& s, unsigned mf = 0, AnimationState state = Undefined, unsigned df = 0)
        : frame { df }, size { s }, maxframe { mf }, animationState { state } {}

    unsigned frame {}; // frame * size of the image = vertical coords of the image
    Nz::Rectui size {};
    unsigned maxframe {};
    AnimationState animationState;

    static Ndk::ComponentIndex componentIndex;
};

#endif // ANIMATIONCOMPONENT_HPP