// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#include <Nazara/Math/Vector2.hpp>
#include <type_traits>
#include "def/typedef.hpp"

extern bool interactSegment(const Vector2fPair& ab, const Vector2fPair& ij, Nz::Vector2f* interactPoint);

template<class T>
inline float getAngleOf(const Nz::Vector2<T>& pA, const Nz::Vector2<T>& pB, const Nz::Vector2<T>& pC);


template<class T>
inline Nz::Vector2<T> makeOrthogonal(const Nz::Vector2<T>& v);

template<class T>
inline bool isLeft(const Nz::Vector2<T>& p1, const Nz::Vector2<T>& p2, const Nz::Vector2<T>& pos);

template<class T>
inline bool isRight(const Nz::Vector2<T>& p1, const Nz::Vector2<T>& p2, const Nz::Vector2<T>& pos);

#include "mathutil.inl"

#endif // MATHUTIL_HPP
