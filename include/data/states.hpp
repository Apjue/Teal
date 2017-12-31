// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATES_HPP
#define STATES_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <utility>
#include <unordered_map>
#include "elementdata.hpp"
#include "def/typedef.hpp"
#include "util/assert.hpp"
#include "util/util.hpp"

struct State
{
    State() = default;

    State(const LuaArguments& args)
    {
        TealException(args.vars.size() >= 4, "Wrong number of arguments. Need at least 4");
        turns = unsigned(args.vars[2].get<double>());
    }

    virtual ~State() = default;

    unsigned turns {};
};

struct PoisonnedState : public State
{
    PoisonnedState() = default;

    PoisonnedState(const LuaArguments& args) : State(args)
    {
        TealException(args.vars.size() == 6, "Wrong number of arguments. Need 6");
        TealAssert(args.vars[3].get<Nz::String>() == getMetadataID(), "Wrong type of state");

        damage.first = stringToElement(args.vars[4].get<Nz::String>());
        damage.second = int(args.vars[5].get<double>());
    }

    std::pair<Element, unsigned> damage;

    static Nz::String getMetadataID()
    {
        return "poison";
    }
};

struct HealedState : public State
{
    HealedState() = default;

    HealedState(const LuaArguments& args) : State(args)
    {
        TealException(args.vars.size() == 6, "Wrong number of arguments. Need 6");
        TealAssert(args.vars[3].get<Nz::String>() == getMetadataID(), "Wrong type of state");

        health.first = stringToElement(args.vars[4].get<Nz::String>());
        health.second = int(args.vars[5].get<double>());
    }

    std::pair<Element, unsigned> health;

    static Nz::String getMetadataID()
    {
        return "heal";
    }
};

struct WeaknessState : public State
{
    WeaknessState() = default;

    WeaknessState(const LuaArguments& args) : State(args)
    {
        NazaraError("not implemented");
    }

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
    PowerState() = default;

    PowerState(const LuaArguments& args) : State(args)
    {
        NazaraError("not implemented");
    }

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
    ParalyzedState() = default;

    ParalyzedState(const LuaArguments& args) : State(args) {}

    static Nz::String getMetadataID()
    {
        return "paralyzed";
    }
};

struct SleepingState : public State // = paralyzed until attacked
{
    SleepingState() = default;

    SleepingState(const LuaArguments& args) : State(args) {}

    static Nz::String getMetadataID()
    {
        return "sleeping";
    }
};

struct ConfusedState : public State // aka drunk
{
    ConfusedState() = default;

    ConfusedState(const LuaArguments& args) : State(args) {}

    static Nz::String getMetadataID()
    {
        return "confused";
    }
};

#endif // STATES_HPP
