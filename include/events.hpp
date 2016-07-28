// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "global.hpp"
#include <anax/Entity.hpp>

namespace Events
{

struct NullEvent {};

struct MouseClick
{
    MouseClick() = default;
    MouseClick(const Button& b, unsigned x_, unsigned y_) : button(b), x(x_), y(y_) {}
    ~MouseClick() = default;

    Button button { Button::RightButton };
    unsigned x{};
    unsigned y{};
};

///
/// \brief The ItemDestroyed struct
///
/// Event used when an item is destroyed
/// So receivers may delete the item from their cache
///

struct ItemDestroyed
{
    ItemDestroyed() = default;
    ItemDestroyed(const ItemDestroyed&) = default;
    ~ItemDestroyed() = default;

    anax::Entity::Id id;
};

// More events will be added later

}

#endif // EVENTS_HPP
