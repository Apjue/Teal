// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATES_HPP
#define STATES_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <utility>
#include <unordered_map>
#include "elementdata.hpp"

struct State
{
    State() = default;
    virtual ~State() = default;

    unsigned turns {};
};

struct PoisonnedState : public State
{
    std::pair<Element, unsigned> damage;

    static Nz::String getMetadataID()
    {
        return "poison";
    }
};

struct HealedState : public State
{
    std::pair<Element, unsigned> health;

    static Nz::String getMetadataID()
    {
        return "heal";
    }
};

struct WeaknessState : public State
{
    std::unordered_map<Element, unsigned> attack;
    std::unordered_map<Element, unsigned> resistance;

    unsigned movementsPoints {};
    unsigned actionPoints {};

    static Nz::String getMetadataID()
    {
        return "weakness";
    }
};

struct PowerState : public State
{
    std::unordered_map<Element, unsigned> attack;
    std::unordered_map<Element, unsigned> resistance;

    unsigned movementsPoints {};
    unsigned actionPoints {};

    static Nz::String getMetadataID()
    {
        return "power";
    }
};

struct ParalyzedState : public State
{
    static Nz::String getMetadataID()
    {
        return "paralyzed";
    }
};

struct SleepingState : public State // = paralyzed until attacked
{
    static Nz::String getMetadataID()
    {
        return "sleeping";
    }
};

struct ConfusedState : public State // aka drunk
{
    static Nz::String getMetadataID()
    {
        return "confused";
    }
};

#endif // STATES_HPP
