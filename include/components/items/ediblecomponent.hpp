// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EDIBLECOMPONENT_HPP
#define EDIBLECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/ObjectHandle.hpp>
#include "def/typedef.hpp"

struct EdibleComponent;
using EdibleComponentHandle = Nz::ObjectHandle<EdibleComponent>;

struct EdibleComponent : public Ndk::Component<EdibleComponent>, public Nz::HandledObject<EdibleComponent>
{
    static inline const char* componentName() { return "edible"; }
    static Ndk::ComponentIndex componentIndex;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, EdibleComponent* component, TypeTag<EdibleComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);
    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, EdibleComponentHandle&& component, TypeTag<EdibleComponentHandle>)
{
    state.PushTable();
    return 1;
}

} // namespace Nz

#endif // EDIBLECOMPONENT_HPP
