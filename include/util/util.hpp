// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UTIL_HPP
#define UTIL_HPP

template<class T>
inline T distance(const T& x, const T& y);


template<class T>
float tofloat(T x);

template<class T>
int toint(T x);

template<class T>
unsigned tounsigned(T x);

template<class T>
long long tolonglong(T x);

#include "util.inl"

#endif // UTIL_HPP
