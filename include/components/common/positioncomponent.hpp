// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef POSITIONCOMPONENT_HPP
#define POSITIONCOMPONENT_HPP

#include <NDK/Component.hpp>
#include "global.hpp"

struct PositionComponent : public Ndk::Component<PositionComponent>
{
    PositionComponent() = default;
    PositionComponent(const AbsTile& pos) : xy { pos } {}

    AbsTile xy {}; // The actual position (absolute)

    DirectionFlags direction; // replaces inXY
    unsigned advancement {};

    static Ndk::ComponentIndex componentIndex;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, PositionComponent* component, TypeTag<PositionComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);
    component->xy = state.CheckField<AbsTile>("xy");

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, PositionComponent&& component, TypeTag<PositionComponent>)
{
    state.PushTable();
    {
        state.PushField("xy", component.xy);
    }

    return 1;
}

} // namespace Nz

#endif // POSITIONCOMPONENT_HPP
