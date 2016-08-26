// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAYBE_HPP
#define MAYBE_HPP

#include <assert.h>

///
/// \brief The Maybe class
///
/// It contains a T object or an invalid T object
/// Check isValid() before using get()
///

template<class T>
class Maybe
{

public:
    Maybe() = default;
    Maybe(T&& t) : m_res { t }, m_valid { true } {}
    ~Maybe() = default;


    Maybe(Maybe&) = delete;
    Maybe& operator=(const Maybe<T>&) = default;
    bool operator==(const Maybe<T>&) = default;

    Maybe(Maybe&&) = default;
    Maybe& operator=(const Maybe<T>&&) = default;
    Maybe& operator=(T&& newT);

    bool isValid() const noexcept;
    const T& get() const;
    T& get();

private:
    T m_res;
    bool m_valid {};
};

#include "maybe.inl"

#endif // MAYBE_HPP

