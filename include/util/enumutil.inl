// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class T>
constexpr T operator&(T a, T b)
{
    return static_cast<T>(static_cast<EnumUnderlyingType<T>>(a) & static_cast<EnumUnderlyingType<T>>(b));
}

template<class T>
constexpr T operator|(T a, T b)
{
    return static_cast<T>(static_cast<EnumUnderlyingType<T>>(a) | static_cast<EnumUnderlyingType<T>>(b));
}


template<class T>
void operator|=(T& a, T b)
{
    a = a | b;
}

template<class T>
void operator&=(T& a, T b)
{
    a = a & b;
}

template<class T>
constexpr T EnumReverse(T a, unsigned flags)
{
    return static_cast<T>(~static_cast<EnumUnderlyingType<T>>(a) &
        (((EnumUnderlyingType<T>(1u) << flags) - 1u)));
}
