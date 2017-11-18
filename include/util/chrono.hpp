// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CHRONO_HPP
#define CHRONO_HPP

#include <chrono>

#if defined(TEAL_LONG_CHRONO)

using Miliseconds = unsigned long long;
using Seconds = unsigned long long;
using Minutes = unsigned long long;

#elif defined(TEAL_TINY_CHRONO)

using Miliseconds = unsigned long;
using Seconds = unsigned;
using Minutes = unsigned short;

#else

using Miliseconds = unsigned long long;
using Seconds = unsigned long;
using Minutes = unsigned;

#endif

using ChronoNowClock = std::chrono::high_resolution_clock;
using ChronoTimePoint = decltype(ChronoNowClock::now());

class Duration
{
public:
    Duration() = default;
    inline Duration(ChronoTimePoint start, ChronoTimePoint end);

    ~Duration() = default;

    inline Miliseconds asMiliseconds() const;
    inline Seconds asSeconds() const;
    inline Minutes asMinutes() const;

private:
    ChronoTimePoint m_start {};
    ChronoTimePoint m_end {};
};

class Chrono
{
public:
    Chrono() = default;
    ~Chrono() = default;

    inline Duration getElapsedTime() const;
    inline void restart();

private:
    ChronoTimePoint m_start { ChronoNowClock::now() };
};

#include "chrono.inl"

#endif // CHRONO_HPP

