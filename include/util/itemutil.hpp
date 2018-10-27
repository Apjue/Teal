// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_ITEMUTIL_HPP
#define TEAL_ITEMUTIL_HPP

#include <NDK/Entity.hpp>

inline bool isItemUsable(Ndk::EntityHandle item);
inline void useItem(Ndk::EntityHandle character, Ndk::EntityHandle item);

#include "itemutil.inl"

#endif // TEAL_ITEMUTIL_HPP
