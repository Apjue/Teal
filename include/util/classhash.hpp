// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CLASSHASH_HPP
#define CLASSHASH_HPP

#include <functional>
#include <utility>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Math/Vector3.hpp>
#include <Nazara/Math/Vector4.hpp>
#include <Nazara/Core/Algorithm.hpp>

namespace std
{

template<class T>
struct hash<Nz::Vector2<T>>;

template<class T>
struct hash<Nz::Vector3<T>>;

template<class T>
struct hash<Nz::Vector4<T>>;

template<class A, class B>
struct hash<std::pair<A, B>>;

}

#include "classhash.inl"

#endif // CLASSHASH_HPP
