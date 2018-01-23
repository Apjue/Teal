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

template<class T>
T distance(const Nz::Vector2<T>& x, const Nz::Vector2<T>& y)
{
    static_assert(std::is_arithmetic<T>::value, "Return type is not arithmetic");

    return T(x.Distancef(y));
}

template<class T>
T distance(const Nz::Vector3<T>& x, const Nz::Vector3<T>& y)
{
    static_assert(std::is_arithmetic<T>::value, "Return type is not arithmetic");

    return T(x.Distancef(y));
}

template<class T>
T root(T x)
{
    return x * x;
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
float getAngleOf(const Vector2Triplet<T>& triangle, unsigned whichAngle) // Al Kashi
{
    T firstSecond = distance(triangle.first, triangle.second);
    T firstThird = distance(triangle.first, triangle.third);
    T secondThird = distance(triangle.second, triangle.third);

    T a {}, b {}, c {}; // cos Â = (b² + c² - a²) / 2bc

    switch (whichAngle)
    {
        case 1:
            a = secondThird;
            b = firstSecond;
            c = firstThird;
            break;

        case 2:
            a = firstThird;
            b = firstSecond;
            c = secondThird;
            break;

        case 3:
            a = firstSecond;
            b = firstThird;
            c = secondThird;
            break;

        default:
            throw std::runtime_error { "Accepted values for whichAngle: 1;2;3" };
    }

    return std::acos(float((root(b) + root(c) - root(a)) / (2 * b * c)));
}
