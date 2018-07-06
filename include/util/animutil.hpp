// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMUTIL_HPP
#define ANIMUTIL_HPP

#include <NDK/Entity.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Math/Vector2.hpp>
#include "components/common/animationcomponent.hpp"

bool hasComponentsToAnimate(const Ndk::EntityHandle& e);
void updateAnimation(const Ndk::EntityHandle& e, float elapsedTime);
void animate(Nz::Vector2ui startCoords, const Nz::SpriteRef& sprite, AnimationData& animData, AnimationComponent::AnimationType animType, std::size_t pathSize);

#endif // ANIMUTIL_HPP
