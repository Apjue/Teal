// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class T>
T distance(const T& x, const T& y)
{
    return (x > y) ? x - y : y - x;
}

inline bool isLineEven(unsigned y)
{
    TealAssert(y <= (Def::ArrayMapY + 1u), "Invalid y !");
    return y % 2 == 0;
}

template<class T>
float tofloat(T x)
{
    return static_cast<float>(x);
}

template<class T>
int toint(T x)
{
    return static_cast<int>(x);
}

template<class T>
unsigned tounsigned(T x)
{
    return static_cast<unsigned>(x);
}

template<class T>
long long tolonglong(T x)
{
    return static_cast<long long>(x);
}
