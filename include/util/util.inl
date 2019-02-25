// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

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
