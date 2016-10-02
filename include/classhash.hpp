// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CLASSHASH_HPP
#define CLASSHASH_HPP

#include <functional>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Core/String.hpp>

namespace std
{

template<class T>
struct hash<Nz::Vector2<T>>;

}

#include "classhash.inl"

#endif // CLASSHASH_HPP
