// Copyright (C) 2018 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef NZSTLCOMPATIBILITY_HPP
#define NZSTLCOMPATIBILITY_HPP

// Nz::Vector2 and std::pair

#include <Nazara/Math/Vector2.hpp>
#include <utility>

template<class T>
std::pair<T, T> toPair(const Nz::Vector2<T>& v);

template<class T>
Nz::Vector2<T> toVector2(const std::pair<T, T>& v);

#include "nzstlcompatibility.inl"

#endif // NZSTLCOMPATIBILITY_HPP
