// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

Miliseconds toMiliseconds(Seconds duration)
{
    return static_cast<Miliseconds>(duration * 1000);
}

Miliseconds toMiliseconds(Minutes duration)
{
    return toMiliseconds(toSeconds(duration));
}


Seconds toSeconds(Miliseconds duration)
{
    return static_cast<Seconds>(duration / 1000);
}

Seconds toSeconds(Minutes duration)
{
    return static_cast<Seconds>(duration * 60);
}


Minutes toMinutes(Miliseconds duration)
{
    return toMinutes(toSeconds(duration));
}

Minutes toMinutes(Seconds duration)
{
    return static_cast<Minutes>(duration / 60);
}



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
