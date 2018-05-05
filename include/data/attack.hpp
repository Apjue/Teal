// Copyright (C) 2018 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <Nazara/Lua/LuaState.hpp>
#include "def/typedef.hpp"

struct Attack
{
    Attack() = default;
    virtual ~Attack() = default;

    enum class AttackType
    {
        Damage,
        State,
        Effect
    };

    static AttackType stringToAttackType(Nz::String string);
    static const char* attackTypeToString(AttackType attackType);

    virtual AttackType getAttackType() = 0;

    enum class Target
    {
        Allies,
        Enemies,
        Both
    } target { Target::Enemies };

    static Target stringToTarget(Nz::String string);
    static const char* targetToString(Target target);
};

#include <Nazara/Lua/LuaState.hpp>
#include <memory>

namespace Nz
{

extern unsigned int LuaImplQueryArg(const LuaState& state, int index, std::shared_ptr<Attack>* attack, TypeTag<std::shared_ptr<Attack>>);
extern int LuaImplReplyVal(const LuaState& state, std::shared_ptr<Attack>&& attack, TypeTag<std::shared_ptr<Attack>>);

} // namespace Nz

#endif // ATTACK_HPP
