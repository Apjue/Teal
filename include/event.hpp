// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

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
        Null,
        MouseClickEvent,
        ItemDestroyedEvent
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

    Event(const Events::MouseClick& e) : clickEvent{e}, m_empty{false}, m_type{MouseClickEvent} {}
    Event(const Events::ItemDestroyed& e) : itemDestroyedEvent(e), m_empty{false}, m_type{ItemDestroyedEvent} {}


    bool operator==(const Type& type) const
    {
        return m_type == type;
    }
    inline bool isValid() const { return !m_empty; }
    inline Type type() const { return m_type; }

    union
    {
        Events::NullEvent null;
        Events::MouseClick clickEvent;
        Events::ItemDestroyed itemDestroyedEvent;
    };
private:
    bool m_empty{true};
    Type m_type{Null};
};

class EventQueue
{
public:
    EventQueue() = default;
    ~EventQueue() = default;

    Event getNext();
    void add(const Event& e);
private:
    static constexpr unsigned m_max{50}; //max events pending.
    std::array<Event, m_max> m_events;
    unsigned m_head{}; //read
    unsigned m_tail{}; //write
};

#endif // EVENT_HPP
