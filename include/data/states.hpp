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
};

struct PoisonnedState : public State
{
    unsigned turns {};
    std::pair<Element, unsigned> damage;

    static Nz::String getMetadataID()
    {
        return "poison";
    }
};

struct RegenerationState : public State
{
    unsigned turns {};
    std::pair<Element, unsigned> health;

    static Nz::String getMetadataID()
    {
        return "regen";
    }
};

#endif // STATES_HPP
