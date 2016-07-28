// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "event.hpp"

Event EventQueue::getNext()
{
    if (m_head == m_tail)
        return Event{};

    unsigned tmphead {m_head};
    m_head = (m_head + 1) % m_max;
    return m_events[tmphead];
}

void EventQueue::add(const Event& e)
{
    assert((m_tail + 1) % m_max != m_head && "Event Queue is full !");

    m_events[m_tail] = e;
    m_tail = (m_tail + 1) % m_max;
}
