// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_STATECOMPONENT_HPP
#define TEAL_STATECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/ObjectHandle.hpp>
#include <vector>
#include "data/statedata.hpp"

struct StateComponent;
using StateComponentHandle = Nz::ObjectHandle<StateComponent>;

struct StateComponent : public Ndk::Component<StateComponent> // States which remain after fight, until next fight
{
    std::vector<StateData> states;

    static inline const char* componentName() { return "states"; }
    static Ndk::ComponentIndex componentIndex;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, StateComponent* component, TypeTag<StateComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);
    component->states = state.Check<std::vector<StateData>>(&index);

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, StateComponentHandle&& component, TypeTag<StateComponentHandle>)
{
    state.Push(component->states);
    return 1;
}

} // namespace Nz

#endif // TEAL_STATECOMPONENT_HPP
