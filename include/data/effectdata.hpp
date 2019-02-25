// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EFFECTDATA_HPP
#define EFFECTDATA_HPP

#include <memory>
#include "util/assert.hpp"
#include "attack.hpp"
#include "effects.hpp"
#include "def/typedef.hpp"

struct EffectData : public Attack
{
    Nz::String effectType;
    std::shared_ptr<Effect> effect;

    virtual AttackType getAttackType() override { return AttackType::Effect; }
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, EffectData* data, TypeTag<EffectData>)
{
    state.CheckType(index, Nz::LuaType_Table);
    TealAssert(Attack::stringToAttackType(state.CheckField<Nz::String>("type", index)) == Attack::AttackType::Effect, "Invalid attack type");

    data->target = Attack::stringToTarget(state.CheckField<Nz::String>("target", index));
    data->effectType = state.CheckField<Nz::String>("effect_type", index);

    {
        if (data->effectType == PullEffect::getMetadataID())
            data->effect = std::make_shared<PullEffect>(state);

        else if (data->effectType == PushEffect::getMetadataID())
            data->effect = std::make_shared<PushEffect>(state);

        else
            throw std::runtime_error { "Invalid effect type" };
    }

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, EffectData&& data, TypeTag<EffectData>)
{
    state.PushTable();
    {
        state.PushField<Nz::String>("type", Attack::attackTypeToString(data.getAttackType()));
        state.PushField<Nz::String>("target", Attack::targetToString(data.target));

        data.effect->serialize(state);
    }

    return 1;
}

} // namespace Nz

#endif // EFFECTDATA_HPP
