// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMATIONSYSTEM_HPP
#define ANIMATIONSYSTEM_HPP

#include <NDK/System.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include "components/common/animationcomponent.hpp"

///
/// \class AnimationSystem
///
/// \brief Animates Graphics Items, as long as
///        they have a sprite attached to them
///

class AnimationSystem : public Ndk::System<AnimationSystem>
{
public:
    AnimationSystem();
    ~AnimationSystem() = default;

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float elapsed) override;
};

#endif // ANIMATIONSYSTEM_HPP
