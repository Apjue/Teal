// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_CONSUMABLECOMPONENT_HPP
#define TEAL_CONSUMABLECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/ObjectHandle.hpp>
#include "def/typedef.hpp"

struct ConsumableComponent;
using ConsumableComponentHandle = Nz::ObjectHandle<ConsumableComponent>;

struct ConsumableComponent : public Ndk::Component<ConsumableComponent>
{
    static inline const char* componentName() { return "consumable"; }
    static Ndk::ComponentIndex componentIndex;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, ConsumableComponent* component, TypeTag<ConsumableComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);
    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, ConsumableComponentHandle&& component, TypeTag<ConsumableComponentHandle>)
{
    state.PushTable();
    return 1;
}

} // namespace Nz

#endif // TEAL_CONSUMABLECOMPONENT_HPP
