// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <cmath>
#include <type_traits>

template<class T, class ReturnType = T>
ReturnType distance(const T& x, const T& y)
{
    static_assert(std::is_arithmetic<ReturnType>::value, "Return type is not arithmetic");

    return x > y ? x - y : y - x;
}

bool isLineEven(unsigned y)
{
    TealAssert(y <= (Def::ArrayMapY + 1u), "Invalid y !");
    return y % 2 == 0;
}

bool interactSegment(const Vector2fPair& ab, const Vector2fPair& ij, Nz::Vector2f* out) // todo: understand this function later.
{
    Nz::Vector2f abVec(ab.second - ab.first), ijVec(ij.second - ij.first);
    float denom(abVec.x * ijVec.y - abVec.y * ijVec.x);

    if (denom == 0)
        return false;

    float u { (ab.first.x * ijVec.y - ij.first.x * ijVec.y - ijVec.x * ab.first.y + ijVec.x * ij.first.y) / denom };
    float v { (-abVec.x * ab.first.y + abVec.x * ij.first.y + abVec.y * ab.first.x - abVec.y * ij.first.x) / denom };

    if (u >= -1 && u <= 0 && v >= -1 && v <= 0)
    {
        if (out != nullptr)
            *out = ab.first - abVec * u;

        return true;
    }

    return false;
}

template<class T>
float getAngleOf(const Nz::Vector2<T>& pA, const Nz::Vector2<T>& pB, const Nz::Vector2<T>& pC) // Get the angle of pA, using the cosine law: cos Â = (b² + c² - a²) / 2bc
{
    float a = pB.Distancef(pC);
    float b = pA.Distancef(pC);
    float c = pA.Distancef(pB);

    return std::acos((b * b + c * c - a * a) / (2.f * b * c));
}
