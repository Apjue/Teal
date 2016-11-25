// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
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
