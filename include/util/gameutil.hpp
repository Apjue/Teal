// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAMEUTIL_HPP
#define GAMEUTIL_HPP

#include <NDK/Entity.hpp>

Ndk::EntityHandle& getMainCharacter();
void initializeGameUtility(Ndk::EntityHandle& mainCharacter);
bool isGameUtilityInitialized();

#endif // GAMEUTIL_HPP
