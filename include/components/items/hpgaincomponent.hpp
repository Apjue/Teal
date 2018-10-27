// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef HPGAINCOMPONENT_HPP
#define HPGAINCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/ObjectHandle.hpp>
#include "def/typedef.hpp"
#include "util/util.hpp"

struct HPGainComponent;
using HPGainComponentHandle = Nz::ObjectHandle<HPGainComponent>;

///
/// \struct HPGainComponent
///
/// \brief Adds 'rel' HPs to the player
///        If rel == 0, the HP of the player becomes 'abs'.
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

inline int LuaImplReplyVal(const LuaState& state, HPGainComponentHandle&& component, TypeTag<HPGainComponentHandle>)
{
    state.PushTable();
    {
        state.PushField("rel", component->rel);
        state.PushField("abs", component->abs);
    }

    return 1;
}

} // namespace Nz

#endif // HPGAINCOMPONENT_HPP
