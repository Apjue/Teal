// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef NZSTLCOMPATIBILITY_HPP
#define NZSTLCOMPATIBILITY_HPP

// Nz::Vector2 and std::pair

#include <utility>
#include <Nazara/Math/Vector2.hpp>

template<class T>
std::pair<T, T> toPair(const Nz::Vector2<T>& v);

template<class T>
Nz::Vector2<T> toVector(const std::pair<T, T>& v);

#include "nzstlcompatibility.inl"

#endif // NZSTLCOMPATIBILITY_HPP
