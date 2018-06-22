// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMUTIL_HPP
#define ANIMUTIL_HPP

#include <NDK/Entity.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include "components/common/animationcomponent.hpp"

bool hasComponentsToAnimate(const Ndk::EntityHandle& e);
void updateAnimation(const Ndk::EntityHandle& e);
void animate(unsigned startX, unsigned startY, const Nz::SpriteRef& sprite, AnimationData& animData, std::size_t pathSize);

#endif // ANIMUTIL_HPP
