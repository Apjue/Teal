// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MOVEMENTUTIL_HPP
#define MOVEMENTUTIL_HPP

#include <NDK/Entity.hpp>

void moveEntity(const Ndk::EntityHandle& e, bool allowMapChange = true);
void recomputeIfObstacle(const Ndk::EntityHandle& e);
void getItemsFromGround(const Ndk::EntityHandle& e);

#endif // MOVEMENTUTIL_HPP
