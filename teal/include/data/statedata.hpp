// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATEDATA_HPP
#define STATEDATA_HPP

#include <Nazara/Core/String.hpp>
#include <memory>
#include <unordered_map>
#include "util/assert.hpp"
#include "data/attack.hpp"
#include "data/states.hpp"
#include "def/typedef.hpp"

struct StateData
{
    unsigned turns {};
    Nz::String metadataId; // To retrieve name/description/icon
    std::unordered_map<StateType, std::shared_ptr<State>> states;
};

struct StateAttackData : public Attack
{
    StateData data;
    virtual AttackType getAttackType() override { return AttackType::State; }
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, StateData* data, TypeTag<StateData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    data->metadataId = state.CheckField<Nz::String>("metadata_id", index);
    data->turns = state.CheckField<unsigned>("turns", index);

    state.GetField("effects", index);

    for (int i { 1 };; ++i)
    {
        state.PushInteger(i);

        if (state.GetTable() != Nz::LuaType_Table)
        {
            state.Pop();
            break;
        }

        StateType stateType = stringToStateType(state.CheckField<Nz::String>("state_type"));

        {
            if (stateType == PoisonnedState::getStateType())
                data->states.emplace(stateType, std::make_shared<PoisonnedState>(state));

            else if (stateType == HealedState::getStateType())
                data->states.emplace(stateType, std::make_shared<HealedState>(state));

            else if (stateType == WeaknessState::getStateType())
                data->states.emplace(stateType, std::make_shared<WeaknessState>(state));

            else if (stateType == PowerState::getStateType())
                data->states.emplace(stateType, std::make_shared<PowerState>(state));

            else if (stateType == ParalyzedState::getStateType())
                data->states.emplace(stateType, std::make_shared<ParalyzedState>(state));

            else if (stateType == SleepingState::getStateType())
                data->states.emplace(stateType, std::make_shared<SleepingState>(state));

            else if (stateType == ConfusedState::getStateType())
                data->states.emplace(stateType, std::make_shared<ConfusedState>(state));

            else
                throw std::runtime_error { "Invalid state type" };
        }

        state.Pop();
    }

    state.Pop();

    return 1;
}

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, StateAttackData* data, TypeTag<StateAttackData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    TealAssert(Attack::stringToAttackType(state.CheckField<Nz::String>("type", index)) == Attack::AttackType::State, "Invalid attack type");
    data->target = Attack::stringToTarget(state.CheckField<Nz::String>("target", index));

    data->data = state.CheckField<StateData>("data", index);

    return 1;
}


inline int LuaImplReplyVal(const LuaState& state, StateData&& data, TypeTag<StateData>)
{
    state.PushTable();
    {
        state.PushField("metadata_id", data.metadataId);
        state.PushField("turns", data.turns);

        state.PushTable();
        {
            unsigned index { 1 };
            for (auto it = data.states.begin(); it != data.states.end(); ++it)
            {
                state.PushTable();
                {
                    it->second->serialize(state);
                }

                state.PushInteger(index);
                state.SetTable();

                ++index;
            }
        }

        state.SetField("effects");
    }

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, StateAttackData&& data, TypeTag<StateAttackData>)
{
    state.PushTable();
    {
        state.PushField<Nz::String>("type", Attack::attackTypeToString(data.getAttackType()));
        state.PushField<Nz::String>("target", Attack::targetToString(data.target));

        state.PushField("data", data.data);
    }

    return 1;
}

} // namespace Nz

#endif // STATEDATA_HPP
