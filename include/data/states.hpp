// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_STATES_HPP
#define TEAL_STATES_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <Nazara/Lua/LuaState.hpp>
#include <utility>
#include <unordered_map>
#include "elementdata.hpp"
#include "def/typedef.hpp"
#include "util/assert.hpp"
#include "util/util.hpp"

struct State
{
    State() = default;
    inline State(const Nz::LuaState& state, int index = -1);

    virtual ~State() = default;

    unsigned turns {};

    virtual inline void serialize(const Nz::LuaState& state);

    struct FightInfo
    {
        std::unordered_map<Element, int> maximumDamage;      // positive: damage, negative: heal
        std::unordered_map<Element, int> attackModifier;     // positive: boost, negative: nerf
        std::unordered_map<Element, int> resistanceModifier; // positive: boost, negative: nerf

        int movementPointsDifference {};
        int actionPointsDifference {};

        enum StateFlag
        {
            None,
            Paralyzed,
            Sleeping,
            Confused
        } flags { None };
    };

    virtual FightInfo getFightInfo() = 0;
};

struct PoisonnedState : public State
{
    inline PoisonnedState(const Nz::LuaState& state, int index = -1);

    std::pair<Element, unsigned> damage;

    virtual inline void serialize(const Nz::LuaState& state) override;
    virtual inline FightInfo getFightInfo() override;
    static const char* getMetadataID() { return "poison"; }
};

struct HealedState : public State
{
    inline HealedState(const Nz::LuaState& state, int index = -1);

    std::pair<Element, unsigned> health;

    virtual inline void serialize(const Nz::LuaState& state) override;
    virtual inline FightInfo getFightInfo() override;
    static const char* getMetadataID() { return "heal"; }
};

struct StatsModifierState : public State
{
    inline StatsModifierState(const Nz::LuaState& state, int index = -1);

    std::unordered_map<Element, int> attack;
    std::unordered_map<Element, int> resistance;

    int movementPoints {};
    int actionPoints {};

    virtual inline void serialize(const Nz::LuaState& state) override;
    virtual inline FightInfo getFightInfo() override;
};

    struct WeaknessState : public StatsModifierState
    {
        inline WeaknessState(const Nz::LuaState& state, int index = -1) : StatsModifierState(state, index) {}
        static const char* getMetadataID() { return "weakness"; }
    };

    struct PowerState : public StatsModifierState
    {
        inline PowerState(const Nz::LuaState& state, int index = -1) : StatsModifierState(state, index) {}
        static const char* getMetadataID() { return "power"; }
    };

struct ParalyzedState : public State
{
    inline ParalyzedState(const Nz::LuaState& state, int index = -1) : State(state, index) {}

    virtual inline FightInfo getFightInfo() override;
    static const char* getMetadataID() { return "paralyzed"; }
};

struct SleepingState : public State // = paralyzed until attacked
{
    inline SleepingState(const Nz::LuaState& state, int index = -1) : State(state, index) {}

    virtual inline FightInfo getFightInfo() override;
    static const char* getMetadataID() { return "sleeping"; }
};

struct ConfusedState : public State // aka drunk
{
    inline ConfusedState(const Nz::LuaState& state, int index = -1) : State(state, index) {}

    virtual inline FightInfo getFightInfo() override;
    static const char* getMetadataID() { return "confused"; }
};

#include "states.inl"

#endif // TEAL_STATES_HPP
