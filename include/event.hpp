// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef EVENT_HPP
#define EVENT_HPP

#include <QString>
#include <array>
#include <tuple>
#include "global.hpp"
#include "events.hpp"

class Event
{
public:
    enum Type
    {
        Undefined,
        ClickEvent
    };

    Event() : null{} {}
    Event(Event&&) = default;
    Event(Event&) = default;

    Event& operator=(const Event&) = default;
    Event& operator=(Event&&) = default;

    ///
    /// \brief operator ==
    ///
    /// Only compares the type of the event.
    /// Not the values.
    ///

    bool operator==(const Event& e) const
    {
        return isValid() == e.isValid() && type() == e.type();
    }
    ~Event() = default;

    Event(const MouseClickEvent& e) : clickEvent{e}, m_empty{false}, m_type{ClickEvent} {}


    bool operator==(const Type& type) const
    {
        return m_type == type;
    }
    inline bool isValid() const { return !m_empty; }
    inline Type type() const { return m_type; }

    union
    {
        NullEvent null;
        MouseClickEvent clickEvent;
    };
private:
    bool m_empty{true};
    Type m_type{Undefined};
};

class EventQueue
{
public:
    EventQueue() = default;
    ~EventQueue() = default;

    Event getNext()
    {
        if (m_head == m_tail) return Event{};

        unsigned tmphead{m_head};
        m_head = (m_head + 1) % m_max;
        return m_events[tmphead];
    }
    void add(const Event& e)
    {
        assert((m_tail + 1) % m_max != m_head && "Event Queue is full !");

        m_events[m_tail] = e;
        m_tail = (m_tail + 1) % m_max;
    }
private:
    const unsigned m_max{50}; //max events pending.
    std::array<Event, 50> m_events;
    unsigned m_head{}; //read
    unsigned m_tail{}; //write
};

#endif // EVENT_HPP
