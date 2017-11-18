// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <vector>
#include "data/animationdata.hpp"

struct AnimationComponent : public Ndk::Component<AnimationComponent>
{
    using AnimationList = std::vector<AnimationData>;
    static constexpr std::size_t InvalidAnimationID = std::numeric_limits<std::size_t>().max();

    AnimationComponent() = default;
    ~AnimationComponent() = default;

    AnimationList animList;
    std::size_t currentAnimation {};

    AnimationData& getCurrentAnimation() { return animList[currentAnimation]; }
    const AnimationData& getCurrentAnimation() const { return animList[currentAnimation]; }

    static Ndk::ComponentIndex componentIndex;
};

#endif // ANIMATIONCOMPONENT_HPP
