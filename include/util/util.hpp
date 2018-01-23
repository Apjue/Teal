// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UTIL_HPP
#define UTIL_HPP

#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Math/Vector3.hpp>
#include "def/gamedef.hpp"
#include "util/assert.hpp"
#include "def/typedef.hpp"

template<class T, class ReturnType>
inline T distance(const T& x, const T& y);

template<class T>
inline T root(T x);

inline bool isLineEven(unsigned y);

inline bool interactSegment(const Vector2fPair& ab, const Vector2fPair& ij, Nz::Vector2f* out);

template<class T>
inline float getAngleOf(const Vector2Triplet<T>& triangle, unsigned whichAngle);

#include "util.inl"

#endif // UTIL_HPP
