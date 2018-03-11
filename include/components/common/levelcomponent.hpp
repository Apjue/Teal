// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LEVELCOMPONENT_HPP
#define LEVELCOMPONENT_HPP

#include <NDK/Component.hpp>

struct LevelComponent : public Ndk::Component<LevelComponent>
{
    LevelComponent(unsigned lvl = 1) : level { lvl } {}

    unsigned level {};

    static Ndk::ComponentIndex componentIndex;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, LevelComponent* component, TypeTag<LevelComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);
    component->level = state.CheckField<unsigned>("level");

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, LevelComponent&& component, TypeTag<LevelComponent>)
{
    state.PushTable();
    {
        state.PushField("level", component.level);
    }

    return 1;
}

} // namespace Nz

#endif // LEVELCOMPONENT_HPP
