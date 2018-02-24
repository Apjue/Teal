// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef DAMAGEMODIFIERCOMPONENT_HPP
#define DAMAGEMODIFIERCOMPONENT_HPP

#include <Nazara/Core/String.hpp>
#include <NDK/Component.hpp>
#include <unordered_map>
#include "data/elementdata.hpp"
#include "def/typedef.hpp"
#include "util/variant.hpp"
#include "util/assert.hpp"
#include "util/util.hpp"

struct DamageModifierComponent : public Ndk::Component<DamageModifierComponent>
{
    std::unordered_map<Element, int> attack; // In percentage
    std::unordered_map<Element, int> resistance;

    static Ndk::ComponentIndex componentIndex;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, DamageModifierComponent* component, TypeTag<DamageModifierComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);

    if (state.GetField("attack", index) == Nz::LuaType_Table)
        for (Element e {}; e <= Element::Max; ++e)
            component->attack[e] = state.CheckField<int>(elementToString(e), 0, -1);

    state.Pop();

    if (state.GetField("resistance", index) == Nz::LuaType_Table)
        for (Element e {}; e <= Element::Max; ++e)
            component->resistance[e] = state.CheckField<int>(elementToString(e), 0, -1);

    state.Pop();

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, DamageModifierComponent&& component, TypeTag<DamageModifierComponent>)
{
    state.PushTable();
    {
        state.PushTable();
        {
            for (Element e {}; e <= Element::Max; ++e)
                state.PushField(elementToString(e), component.attack[e]);
        }

        state.SetField("attack");


        state.PushTable();
        {
            for (Element e {}; e <= Element::Max; ++e)
                state.PushField(elementToString(e), component.resistance[e]);
        }

        state.SetField("resistance");
    }

    return 1;
}

} // namespace Nz

#endif // DAMAGEMODIFIERCOMPONENT_HPP
