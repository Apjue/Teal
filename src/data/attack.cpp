// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "data/damagedata.hpp"
#include "data/statedata.hpp"
#include "data/effectdata.hpp"
#include "data/attack.hpp"

Attack::AttackType Attack::stringToAttackType(Nz::String string)
{
    string = string.ToLower();

    if (string == "damage")
        return AttackType::Damage;

    if (string == "state")
        return AttackType::State;

    if (string == "effect")
        return AttackType::Effect;

    throw std::runtime_error { "Invalid attack type" };
}

const char* Attack::attackTypeToString(AttackType attackType)
{
    switch (attackType)
    {
        case Attack::AttackType::Damage:
            return "damage";

        case Attack::AttackType::State:
            return "state";

        case Attack::AttackType::Effect:
            return "effect";
    }

    throw std::runtime_error { "Invalid attack type" };
}


Attack::Target Attack::stringToTarget(Nz::String string)
{
    string = string.ToLower();

    if (string == "allies")
        return Target::Allies;

    if (string == "enemies")
        return Target::Enemies;

    if (string == "both")
        return Target::Both;

    throw std::runtime_error { "Invalid target" };
}

const char* Attack::targetToString(Target target)
{
    switch (target)
    {
        case Target::Allies:
            return "allies";

        case Target::Enemies:
            return "enemies";

        case Target::Both:
            return "both";
    }

    throw std::runtime_error { "Invalid target" };
}

namespace Nz
{

unsigned int LuaImplQueryArg(const LuaState& state, int index, std::shared_ptr<Attack>* attack, TypeTag<std::shared_ptr<Attack>>)
{
    state.CheckType(index, Nz::LuaType_Table); // todo: fix crash: attack can be nullptr^

    Attack::AttackType attackType = Attack::stringToAttackType(state.CheckField<Nz::String>("type", index));
    bool initializePtr = (attack->get() == nullptr);

    switch (attackType)
    {
        case Attack::AttackType::Damage:
            if (initializePtr)
                attack->reset(std::make_unique<DamageData>().release());

            return LuaImplQueryArg(state, index, static_cast<DamageData*>(attack->get()), TypeTag<DamageData>());

        case Attack::AttackType::State:
            if (initializePtr)
                attack->reset(std::make_unique<StateData>().release());

            return LuaImplQueryArg(state, index, static_cast<StateData*>(attack->get()), TypeTag<StateData>());

        case Attack::AttackType::Effect:
            if (initializePtr)
                attack->reset(std::make_unique<EffectData>().release());

            return LuaImplQueryArg(state, index, static_cast<EffectData*>(attack->get()), TypeTag<EffectData>());

        default:
            throw std::runtime_error { "Invalid attack type" };
    }
}

inline int LuaImplReplyVal(const LuaState& state, std::shared_ptr<Attack>&& attack, TypeTag<std::shared_ptr<Attack>>)
{
    switch (attack->getAttackType())
    {
        case Attack::AttackType::Damage:
            return LuaImplReplyVal(state, std::move(*static_cast<DamageData*>(attack.get())), TypeTag<DamageData>());

        case Attack::AttackType::State:
            return LuaImplReplyVal(state, std::move(*static_cast<StateData*>(attack.get())), TypeTag<StateData>());

        case Attack::AttackType::Effect:
            return LuaImplReplyVal(state, std::move(*static_cast<EffectData*>(attack.get())), TypeTag<EffectData>());
    }

    throw std::runtime_error { "Invalid attack type" };
}

} // namespace Nz
