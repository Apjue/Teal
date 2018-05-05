// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATES_INL
#define STATES_INL

#include "states.hpp"

State::State(const Nz::LuaState& state, int index)
{
    turns = state.CheckField<unsigned>("turns", index);
}

void State::serialize(const Nz::LuaState& state)
{
    state.PushField("turns", turns);
}

PoisonnedState::PoisonnedState(const Nz::LuaState& state, int index) : State(state, index)
{
    damage.first = state.CheckField<Element>("element", index);
    damage.second = state.CheckField<unsigned>("damage", index);
}

void PoisonnedState::serialize(const Nz::LuaState& state)
{
    State::serialize(state);

    state.PushField("element", damage.first);
    state.PushField("damage", damage.second);
}

State::FightInfo PoisonnedState::getFightInfo()
{
    FightInfo info;
    info.maximumDamage[damage.first] = damage.second;

    return info;
}

HealedState::HealedState(const Nz::LuaState& state, int index) : State(state, index)
{
    health.first = state.CheckField<Element>("element", index);
    health.second = state.CheckField<unsigned>("health", index);
}

void HealedState::serialize(const Nz::LuaState& state)
{
    State::serialize(state);

    state.PushField("element", health.first);
    state.PushField("health", health.second);
}

State::FightInfo HealedState::getFightInfo()
{
    FightInfo info;
    info.maximumDamage[health.first] = health.second;

    return info;
}

StatsModifierState::StatsModifierState(const Nz::LuaState& state, int index) : State(state, index)
{
    if (state.GetField("attack", index) == Nz::LuaType_Table)
        for (Element e {}; e <= Element::Max; ++e)
            attack[e] = state.CheckField<int>(elementToString(e), 0, -1);

    state.Pop();

    if (state.GetField("resistance", index) == Nz::LuaType_Table)
        for (Element e {}; e <= Element::Max; ++e)
            resistance[e] = state.CheckField<int>(elementToString(e), 0, -1);

    state.Pop();

    movementPoints = state.CheckField<int>("movement_points", 0, index);
    actionPoints = state.CheckField<int>("action_points", 0, index);
}

void StatsModifierState::serialize(const Nz::LuaState& state)
{
    State::serialize(state);

    state.PushTable();
    {
        for (Element e {}; e <= Element::Max; ++e)
            state.PushField(elementToString(e), attack[e]);
    }

    state.SetField("attack");


    state.PushTable();
    {
        for (Element e {}; e <= Element::Max; ++e)
            state.PushField(elementToString(e), resistance[e]);
    }

    state.SetField("resistance");
}

State::FightInfo StatsModifierState::getFightInfo()
{
    FightInfo info;

    info.attackModifier = attack;
    info.resistanceModifier = resistance;

    info.movementPointsDifference = movementPoints;
    info.actionPointsDifference = actionPoints;

    return info;
}


State::FightInfo ParalyzedState::getFightInfo()
{
    FightInfo info;
    info.flags = FightInfo::Paralyzed;

    return info;
}

State::FightInfo SleepingState::getFightInfo()
{
    FightInfo info;
    info.flags = FightInfo::Sleeping;

    return info;
}

State::FightInfo ConfusedState::getFightInfo()
{
    FightInfo info;
    info.flags = FightInfo::Confused;

    return info;
}

#endif // STATES_INL
