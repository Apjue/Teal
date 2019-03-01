// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "characterdata.hpp"

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, CharacterData::RandomMovement* random, TypeTag<CharacterData::RandomMovement>)
{
    if (state.GetType(index) != Nz::LuaType_Table)
        return 1;

    random->enabled = true;
    random->movementInterval = state.CheckField<float>("movement_interval", index);
    random->range = state.CheckField<unsigned>("range", index);

    return 1;
}

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, CharacterData* data, TypeTag<CharacterData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    LuaImplQueryArg(state, index, &data->livingEntityData, TypeTag<LivingEntityData> {});
    data->blockTile = state.CheckField<bool>("blocktile", false, index);
    data->randomMovement = state.CheckField<CharacterData::RandomMovement>("random_movement", index); // Not sure about this

    return 1;
}

}
