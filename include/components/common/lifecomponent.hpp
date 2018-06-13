// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LIFECOMPONENT_HPP
#define LIFECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/ObjectHandle.hpp>

struct LifeComponent;
using LifeComponentHandle = Nz::ObjectHandle<LifeComponent>;


struct LifeComponent : public Ndk::Component<LifeComponent>, public Nz::HandledObject<LifeComponent>
{
    unsigned hp { 100 };
    unsigned maxHp { 100 };

    static Ndk::ComponentIndex componentIndex;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, LifeComponent* component, TypeTag<LifeComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);

    component->hp = state.CheckField<unsigned>("hp");
    component->maxHp = state.CheckField<unsigned>("max_hp");

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, LifeComponentHandle&& component, TypeTag<LifeComponentHandle>)
{
    state.PushTable();
    {
        state.PushField("hp", component->hp);
        state.PushField("max_hp", component->maxHp);
    }

    return 1;
}

} // namespace Nz

#endif // LIFECOMPONENT_HPP
