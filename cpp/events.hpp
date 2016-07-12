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

#endif // EVENTS_HPP
