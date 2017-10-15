// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMUTIL_HPP
#define ANIMUTIL_HPP

#include <NDK/Entity.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include "components/common/animationcomponent.hpp"

bool hasRightComponentsToAnimate(const Ndk::EntityHandle& e);
void updateAnimation(const Ndk::EntityHandle& e);
void animate(unsigned startX, unsigned startY, Nz::SpriteRef sprite, AnimationData& animData, bool moving);

#endif // ANIMUTIL_HPP
