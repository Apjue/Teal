// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
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
    virtual ~State() = default;

    virtual void serialize(const Nz::LuaState& state) = 0;

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

enum class StateType
{
    PoisonnedState,
    HealedState,
    WeaknessState,
    PowerState,
    ParalyzedState,
    SleepingState,
    ConfusedState
};

inline Nz::String stateTypeToString(StateType stateType);
inline StateType stringToStateType(Nz::String string);


struct PoisonnedState : public State
{
    inline PoisonnedState(const Nz::LuaState& state, int index = -1);

    std::pair<Element, unsigned> damage;

    virtual inline void serialize(const Nz::LuaState& state) override;
    virtual inline FightInfo getFightInfo() override;
    static StateType getStateType() { return StateType::PoisonnedState; }
};

struct HealedState : public State
{
    inline HealedState(const Nz::LuaState& state, int index = -1);

    std::pair<Element, unsigned> health;

    virtual inline void serialize(const Nz::LuaState& state) override;
    virtual inline FightInfo getFightInfo() override;
    static StateType getStateType() { return StateType::HealedState; }
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

        virtual inline void serialize(const Nz::LuaState& state) override;
        static StateType getStateType() { return StateType::WeaknessState; }
    };

    struct PowerState : public StatsModifierState
    {
        inline PowerState(const Nz::LuaState& state, int index = -1) : StatsModifierState(state, index) {}

        virtual inline void serialize(const Nz::LuaState& state) override;
        static StateType getStateType() { return StateType::PowerState; }
    };

struct ParalyzedState : public State
{
    inline ParalyzedState(const Nz::LuaState& state, int index = -1) {}

    virtual inline void serialize(const Nz::LuaState& state) override;
    virtual inline FightInfo getFightInfo() override;
    static StateType getStateType() { return StateType::ParalyzedState; }
};

struct SleepingState : public State // = paralyzed until attacked
{
    inline SleepingState(const Nz::LuaState& state, int index = -1) {}

    virtual inline void serialize(const Nz::LuaState& state) override;
    virtual inline FightInfo getFightInfo() override;
    static StateType getStateType() { return StateType::SleepingState; }
};

struct ConfusedState : public State // aka drunk
{
    inline ConfusedState(const Nz::LuaState& state, int index = -1) {}

    virtual inline void serialize(const Nz::LuaState& state) override;
    virtual inline FightInfo getFightInfo() override;
    static StateType getStateType() { return StateType::ConfusedState; }
};

#include "states.inl"

#endif // TEAL_STATES_HPP
