// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CHRONO_INL
#define CHRONO_INL

Duration::Duration(ChronoTimePoint start, ChronoTimePoint end)
    : m_start { start }, m_end { end } {}

Miliseconds Duration::asMiliseconds() const
{
    return static_cast<Miliseconds>(std::chrono::duration_cast<std::chrono::milliseconds>
        (m_end - m_start).count());
}

Seconds Duration::asSeconds() const
{
    return static_cast<Seconds>(std::chrono::duration_cast<std::chrono::seconds>
        (m_end - m_start).count());
}

Minutes Duration::asMinutes() const
{
    return static_cast<Minutes>(std::chrono::duration_cast<std::chrono::minutes>
        (m_end - m_start).count());
}


Duration Chrono::getElapsedTime() const
{
    return Duration { m_start, ChronoNowClock::now() };
}

void Chrono::restart()
{
    m_start = ChronoNowClock::now();
}

#endif // CHRONO_INL