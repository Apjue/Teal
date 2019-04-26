// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_MONSTERDATA_HPP
#define TEAL_MONSTERDATA_HPP

#include <Nazara/Core/String.hpp>
#include "data/livingentitydata.hpp"

struct MonsterData // Contains all required attributes of a monster. Used by a monster factory.
{
    MonsterData() = default;
    ~MonsterData() = default;

    LivingEntityData livingEntityData;
    Nz::String monsterFamily;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, MonsterData* data, TypeTag<MonsterData>);

} // namespace Nz

#include "monsterdata.inl"

#endif // TEAL_MONSTERDATA_HPP
