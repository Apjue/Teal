// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef DAMAGEDATA_HPP
#define DAMAGEDATA_HPP

#include <utility>
#include "elementdata.hpp"
#include "attack.hpp"
#include "def/typedef.hpp"
#include "util/util.hpp"

struct DamageData : public Attack
{
    std::pair<Element, int> damage;

    virtual AttackType getAttackType() override { return AttackType::Damage; }
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, DamageData* data, TypeTag<DamageData>)
{
    state.CheckType(index, Nz::LuaType_Table);
    TealAssert(Attack::stringToAttackType(state.CheckField<Nz::String>("type", index)) == Attack::AttackType::Damage, "Invalid attack type");

    data->target = Attack::stringToTarget(state.CheckField<Nz::String>("target", -1));

    data->damage.first = state.CheckField<Element>("element", -1);
    data->damage.second = state.CheckField<int>("damage", -1);

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, DamageData&& data, TypeTag<DamageData>)
{
    state.PushTable();
    {
        state.PushField<Nz::String>("type", Attack::attackTypeToString(data.getAttackType()));
        state.PushField<Nz::String>("target", Attack::targetToString(data.target));

        state.PushField("element", data.damage.first);
        state.PushField("damage", data.damage.second);
    }

    return 1;
}

} // namespace Nz

#endif // DAMAGEDATA_HPP
