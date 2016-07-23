// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <utility>

template<class T, class T2 = T>
class Vector2
{
    std::pair<T, T2> m_pair;
public:
    Vector2() = default;
    Vector2(const T& first_, const T2& second_)
        : m_pair{ first_, second_ }
    {}
    ~Vector2() = default;

    Vector2(const Vector2<T, T2>&) = default;
    Vector2& operator=(const Vector2<T, T2>&) = default;

    Vector2(Vector2<T, T2>&&) = default;
    Vector2& operator=(Vector2<T, T2>&&) = default;

    const T& first() const
    {
        return m_pair.first;
    }
    T& first()
    {
        return m_pair.first;
    }

    const T2& second() const
    {
        return m_pair.second;
    }
    T2& second()
    {
        return m_pair.second;
    }

    friend Vector2& operator+ (const Vector2<T, T2>& lhs, const Vector2<T, T2>& rhs)
    {
        return { lhs.first()  + rhs.first(),
                 lhs.second() + rhs.second() };
    }
    friend Vector2& operator- (const Vector2<T, T2>& lhs, const Vector2<T, T2>& rhs)
    {
        return { lhs.first()  - rhs.first(),
                 lhs.second() - rhs.second() };
    }
    friend Vector2& operator* (const Vector2<T, T2>& lhs, const Vector2<T, T2>& rhs)
    {
        return { lhs.first()  * rhs.first(),
                 lhs.second() * rhs.second() };
    }
    friend Vector2& operator/ (const Vector2<T, T2>& lhs, const Vector2<T, T2>& rhs)
    {
        return { lhs.first()  / rhs.first(),
                 lhs.second() / rhs.second() };
    }

    Vector2& operator+= (const Vector2<T, T2>& other)
    {
        operator=(operator+(*this, other));
        return *this;
    }
    Vector2& operator-= (const Vector2<T, T2>& other)
    {
        operator=(operator-(*this, other));
        return *this;
    }
    Vector2& operator*= (const Vector2<T, T2>& other)
    {
        operator=(operator*(*this, other));
        return *this;
    }
    Vector2& operator/= (const Vector2<T, T2>& other)
    {
        operator=(operator/(*this, other));
        return *this;
    }
};

using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

#endif // VECTOR2_HPP
