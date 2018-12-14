// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_ANIMATIONCOMPONENT_INL
#define TEAL_ANIMATIONCOMPONENT_INL

#include "animationcomponent.hpp"

AnimationComponent::AnimationType AnimationComponent::stringToAnimationType(Nz::String string)
{
    string = string.ToLower();

    if (string == "walk")
        return Walk;

    if (string == "run")
        return Run;

    if (string == "fight")
        return Fight;

    if (string == "emote")
        return Emote;

    throw std::runtime_error { "Undefined animation type!" };
}

Nz::String AnimationComponent::animationTypeToString(AnimationType animType)
{
    switch (animType)
    {
        case Walk:
            return "walk";

        case Run:
            return "run";

        case Fight:
            return "fight";

        case Emote:
            return "emote";
    }

    throw std::runtime_error { "Undefined animation type!" };
}

bool AnimationComponent::canAnimationBeUsed(AnimationType type)
{
    return animList.find(type) != animList.end();
}

#endif // TEAL_ANIMATIONCOMPONENT_INL
