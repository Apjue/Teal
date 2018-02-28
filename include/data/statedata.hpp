// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATEDATA_HPP
#define STATEDATA_HPP

#include <memory>
#include "util/assert.hpp"
#include "attack.hpp"
#include "states.hpp"
#include "def/typedef.hpp"

struct StateData : public Attack
{
    Nz::String stateType;
    std::shared_ptr<State> state;

    virtual AttackType getAttackType() override { return AttackType::State; }
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, StateData* data, TypeTag<StateData>)
{
    state.CheckType(index, Nz::LuaType_Table);
    TealAssert(Attack::stringToAttackType(state.CheckField<Nz::String>("type", index)) == Attack::AttackType::State, "Invalid attack type");

    data->target = Attack::stringToTarget(state.CheckField<Nz::String>("target", index));
    data->stateType = state.CheckField<Nz::String>("state_type", index);

    {
        if (data->stateType == PoisonnedState::getMetadataID())
            data->state = std::make_shared<PoisonnedState>(state);

        else if (data->stateType == HealedState::getMetadataID())
            data->state = std::make_shared<HealedState>(state);

        else if (data->stateType == WeaknessState::getMetadataID())
            data->state = std::make_shared<WeaknessState>(state);

        else if (data->stateType == PowerState::getMetadataID())
            data->state = std::make_shared<PowerState>(state);

        else if (data->stateType == ParalyzedState::getMetadataID())
            data->state = std::make_shared<ParalyzedState>(state);

        else if (data->stateType == SleepingState::getMetadataID())
            data->state = std::make_shared<SleepingState>(state);

        else if (data->stateType == ConfusedState::getMetadataID())
            data->state = std::make_shared<ConfusedState>(state);

        else
            throw std::runtime_error { "Invalid state type" };
    }

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, StateData&& data, TypeTag<StateData>)
{
    state.PushTable();
    {
        state.PushField<Nz::String>("type", Attack::attackTypeToString(data.getAttackType()));
        state.PushField<Nz::String>("target", Attack::targetToString(data.target));

        data.state->serialize(state);
    }

    return 1;
}

} // namespace Nz

#endif // STATEDATA_HPP
