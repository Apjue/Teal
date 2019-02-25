// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CLASSHASH_HPP
#define CLASSHASH_HPP

#include <functional>
#include <utility>
#include <Nazara/Core/Algorithm.hpp>

namespace std
{

template<class A, class B>
struct hash<std::pair<A, B>>;

}

#include "classhash.inl"

#endif // CLASSHASH_HPP
