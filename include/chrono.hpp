// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CHRONO_HPP
#define CHRONO_HPP

#include <chrono>

using Miliseconds = unsigned long long;
#ifdef TEAL_LONG_CHRONO
using Seconds = unsigned long long;
using Minutes = unsigned long;
#else
using Seconds = unsigned long;
using Minutes = unsigned;
#endif

class ElapsedTime
{
    using NowClock = std::chrono::high_resolution_clock;

public:
    ElapsedTime() = default;
    ElapsedTime(decltype(NowClock::now()) start, decltype(NowClock::now()) end)
        : m_start{start}, m_end{end} {}

    ~ElapsedTime() = default;

    inline Miliseconds asMiliseconds() const
    {
        return static_cast<Miliseconds>(std::chrono::duration_cast<std::chrono::milliseconds>
                                               (m_end - m_start).count());
    }
    inline Seconds asSeconds() const
    {
        return static_cast<Seconds>(std::chrono::duration_cast<std::chrono::seconds>
                                               (m_end - m_start).count());
    }
    inline Minutes asMinutes() const
    {
        return static_cast<Minutes>(std::chrono::duration_cast<std::chrono::minutes>
                                         (m_end - m_start).count());
    }

private:
    decltype(NowClock::now()) m_start{};
    decltype(NowClock::now()) m_end{};
};

class Chrono
{
    using NowClock = std::chrono::high_resolution_clock;
public:
    Chrono() : m_start{ NowClock::now() } {}
    ~Chrono() = default;

    inline ElapsedTime getElapsedTime() const
    {
        return ElapsedTime{m_start, NowClock::now()};
    }
    inline void restart()
    {
        m_start = NowClock::now();
    }

private:
    decltype(NowClock::now()) m_start{ NowClock::now() };
};

#endif // CHRONO_HPP
