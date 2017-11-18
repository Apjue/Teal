// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UTIL_HPP
#define UTIL_HPP

#include "def/gamedef.hpp"
#include "util/assert.hpp"

template<class T>
inline T distance(const T& x, const T& y);

inline bool isLineEven(unsigned y);

#include "util.inl"

#endif // UTIL_HPP
