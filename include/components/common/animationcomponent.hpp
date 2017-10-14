// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <vector>
#include "cache/doublestore.hpp"

///
/// \todo Do not have a "maxframe" variable. Deduce maxframe from texture size / sprite size
///       Have multiple animations (eg for attacks). AnimationStore ?
///

struct AnimationComponent : public Ndk::Component<AnimationComponent>
{
    using AnimationList = std::vector<AnimationData>;
    static constexpr std::size_t InvalidAnimationID = std::numeric_limits<std::size_t>().max();

    AnimationComponent() = default;
    ~AnimationComponent() = default;

    AnimationList anims;
    std::size_t currentAnim {};

    AnimationData& getCurrentAnim() { return anims[currentAnim]; }
    const AnimationData& getCurrentAnim() const { return anims[currentAnim]; }

    static Ndk::ComponentIndex componentIndex;
};

#endif // ANIMATIONCOMPONENT_HPP
