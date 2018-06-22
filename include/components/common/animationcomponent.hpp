// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMATIONCOMPONENT_HPP
#define ANIMATIONCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include "data/animationdata.hpp"

struct AnimationComponent : public Ndk::Component<AnimationComponent>
{
    AnimationComponent() = default;
    ~AnimationComponent() = default;


    enum AnimationType
    {
        Walk, // In map, normal animation ; Also used when Idle
        Run,  // ^ Same as walk, but... when running. (no shit)
        Fight,
        Emote /// todo: implement emotes thing | am i going to need a vector with animtypes? for priority, etc. (e.g. emote while running?)
    };

    static inline AnimationType stringToAnimationType(Nz::String string);
    static inline Nz::String animationTypeToString(AnimationType animType);


    using AnimationList = std::unordered_map<AnimationType, AnimationData>;
    AnimationList animList;
    
    inline bool canAnimationBeUsed(AnimationType type);

    static Ndk::ComponentIndex componentIndex;
};

#include "animationcomponent.inl"

#endif // ANIMATIONCOMPONENT_HPP
