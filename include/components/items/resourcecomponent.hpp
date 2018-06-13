// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RESOURCECOMPONENT_HPP
#define RESOURCECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/ObjectHandle.hpp>
#include "def/typedef.hpp"

struct ResourceComponent;
using ResourceComponentHandle = Nz::ObjectHandle<ResourceComponent>;

struct ResourceComponent : public Ndk::Component<ResourceComponent>, public Nz::HandledObject<ResourceComponent>
{
    static inline const char* componentName() { return "resource"; }
    static Ndk::ComponentIndex componentIndex;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, ResourceComponent* component, TypeTag<ResourceComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);
    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, ResourceComponentHandle&& component, TypeTag<ResourceComponentHandle>)
{
    state.PushTable();
    return 1;
}

} // namespace Nz

#endif // RESOURCECOMPONENT_HPP
