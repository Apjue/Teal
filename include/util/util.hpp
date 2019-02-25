// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UTIL_HPP
#define UTIL_HPP

#include "util/assert.hpp"
#include "def/gamedef.hpp"

template<class T, class ReturnType>
inline T distance(const T& x, const T& y);

inline bool isLineEven(unsigned y);

#include "util.inl"

#endif // UTIL_HPP
