// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "global.hpp"

struct NullEvent {};

struct MouseClickEvent
{
    MouseClickEvent() = default;
    MouseClickEvent(const Button& b, unsigned x_, unsigned y_) : button(b), x(x_), y(y_) {}
    ~MouseClickEvent() = default;

    Button button{Button::RightButton};
    unsigned x{};
    unsigned y{};
};

// More events will be added later

#endif // EVENTS_HPP
