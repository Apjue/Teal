// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPUTIL_HPP
#define MAPUTIL_HPP

#include <NDK/Entity.hpp>

bool hasComponentsToChangeMap(const Ndk::EntityHandle& p);

bool canChangeMap(const Ndk::EntityHandle& p);

void changeMap(const Ndk::EntityHandle& p);

void initMapUtility();

#include "maputil.inl"

#endif // MAPUTIL_HPP
