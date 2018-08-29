// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <string>
#include "cache/doublestore.hpp"
#include "components/items/equippablecomponent.hpp"

namespace Nz
{

unsigned int LuaImplQueryArg(const LuaState& state, int index, EquippableComponent* component, TypeTag<EquippableComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);

    component->bodypart = stringToBodypart(state.CheckField<Nz::String>("bodypart", index));
    component->side = stringToSide(state.CheckField<Nz::String>("side", index));

    if (state.GetField("skill_id", index) == Nz::LuaType_String)
    {
        const char* skillName = state.CheckString(-1);
        component->attackId = DoubleStores<SkillData>::getInstance()->getItemIndex(skillName);
    }

    state.Pop();

    return 1;
}

int LuaImplReplyVal(const LuaState& state, EquippableComponentHandle&& component, TypeTag<EquippableComponentHandle>)
{
    state.PushTable();
    {
        state.PushField<Nz::String>("bodypart", bodypartToString(component->bodypart));
        state.PushField<Nz::String>("side", sideToString(component->side));

        if (component->attackId != SkillStore::InvalidID)
            state.PushField("skill_id", DoubleStores<SkillData>::getInstance()->getItem(component->attackId).codename);
    }

    return 1;
}

} // namespace Nz
