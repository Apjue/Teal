// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <cmath>
#include "mathutil.hpp"

template<class T>
float getAngleOf(const Nz::Vector2<T>& pA, const Nz::Vector2<T>& pB, const Nz::Vector2<T>& pC) // Get the angle of pA
{
    float a = pB.Distancef(pC);
    float b = pA.Distancef(pC);
    float c = pA.Distancef(pB);

    return std::acos((b * b + c * c - a * a) / (2.f * b * c)); // Law of cosines (Al Kashi): cos Â = (b² + c² - a²) / 2bc
}

template<class T>
Nz::Vector2<T> makeOrthogonal(const Nz::Vector2<T>& v)
{
    static_assert(std::is_signed<T>::value, "operator- on an unsigned ?");

    return Nz::Vector2<T> { v.y, -v.x };
}

template<class T>
bool isLeft(const Nz::Vector2<T>& p1, const Nz::Vector2<T>& p2, const Nz::Vector2<T>& pos)
{
    return Nz::Vector2<T>::DotProduct(p2 - p1, makeOrthogonal(pos - p2)) > 0;
}

template<class T>
bool isRight(const Nz::Vector2<T>& p1, const Nz::Vector2<T>& p2, const Nz::Vector2<T>& pos)
{
    return Nz::Vector2<T>::DotProduct(p2 - p1, makeOrthogonal(pos - p2)) < 0;
}
