// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ENTITYUTIL_HPP
#define ENTITYUTIL_HPP

#include <NDK/Entity.hpp>
#include "components/common/animationcomponent.hpp"

inline bool isMapEntity(const Ndk::EntityHandle& e); // Check if the entity is a map object (e.g. a tree or a NPC) or an item laying on the ground
inline bool isItemEntity(const Ndk::EntityHandle& e);
inline bool isMonsterEntity(const Ndk::EntityHandle& e);
inline bool isFightableEntity(const Ndk::EntityHandle& e);

inline bool isGraphicalItemEntity(const Ndk::EntityHandle& e);
inline bool isValidGraphicalItemEntity(const Ndk::EntityHandle& e);

inline bool isEntityMoving(const Ndk::EntityHandle& e);
inline bool isEntityRunning(const Ndk::EntityHandle& e);
inline bool hasComponentsToChangeMap(const Ndk::EntityHandle& e);
inline Nz::Vector2f getDefGfxPos(const Ndk::EntityHandle& e); // Animations may have different offset than default character offset
inline AnimationComponent::AnimationType determineAnimationToBeUsed(const Ndk::EntityHandle& e);

#include "entityutil.inl"

#endif // ENTITYUTIL_HPP
