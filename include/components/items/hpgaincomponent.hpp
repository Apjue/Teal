// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef HPGAINCOMPONENT_HPP
#define HPGAINCOMPONENT_HPP

#include <NDK/Component.hpp>
#include "def/typedef.hpp"
#include "util/util.hpp"

///
/// \struct HPGainComponent
///
/// \brief Adds diff HPs to the player
///        If diff == 0, the HP of the player becomes abs.
///

struct HPGainComponent : public Ndk::Component<HPGainComponent>
{
    int rel {}; // can be negative, e.g. for poison
    unsigned abs {};

    static inline const char* componentName() { return "hpgain"; }
    static Ndk::ComponentIndex componentIndex;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, HPGainComponent* component, TypeTag<HPGainComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);

    component->rel = state.CheckField<int>("rel");
    component->abs = state.CheckField<unsigned>("abs");

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, HPGainComponent&& component, TypeTag<HPGainComponent>)
{
    state.PushTable();
    {
        state.PushField("rel", component.rel);
        state.PushField("abs", component.abs);
    }

    return 1;
}

} // namespace Nz

#endif // HPGAINCOMPONENT_HPP
