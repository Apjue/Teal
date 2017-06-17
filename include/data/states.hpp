// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATES_HPP
#define STATES_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <utility>
#include "elementdata.hpp"

struct State
{
    State() = default;
    virtual ~State() = default;

    Nz::String name;
    Nz::String description; // Text description. Fight description will be auto generated
    Nz::TextureRef icon; // For player
};

struct PoisonnedState : public State
{
    unsigned turns {};
    std::pair<Element, unsigned> damage;
};

struct RegenerationState : public State
{
    unsigned turns {};
    std::pair<Element, unsigned> health;
};

#endif // STATES_HPP
