// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MOVEMENTUTIL_HPP
#define MOVEMENTUTIL_HPP

#include <NDK/Entity.hpp>

void moveEntity(const Ndk::EntityHandle& e, bool fightMode = false);
void recomputePathIfObstacle(const Ndk::EntityHandle& e); // Recomputes path of entity if a NPC moved into it
void getItemsFromGround(const Ndk::EntityHandle& e);

#endif // MOVEMENTUTIL_HPP
