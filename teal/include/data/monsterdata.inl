// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/LuaAPI.hpp>
#include "monsterdata.hpp"

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, MonsterData* data, TypeTag<MonsterData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    LuaImplQueryArg(state, index, &data->livingEntityData, TypeTag<LivingEntityData> {});
    data->monsterFamily = state.CheckField<Nz::String>("family");

    return 1;
}

}
