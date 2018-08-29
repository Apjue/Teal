// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EQUIPPABLECOMPONENT_HPP
#define EQUIPPABLECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/ObjectHandle.hpp>
#include <Nazara/Lua/LuaState.hpp>
#include "cache/doublestore.hpp"
#include "data/equipmentdata.hpp"
#include "def/typedef.hpp"

struct EquippableComponent;
using EquippableComponentHandle = Nz::ObjectHandle<EquippableComponent>;

namespace Nz
{

extern unsigned int LuaImplQueryArg(const LuaState& state, int index, EquippableComponent* component, TypeTag<EquippableComponent>);
extern int LuaImplReplyVal(const LuaState& state, EquippableComponentHandle&& component, TypeTag<EquippableComponentHandle>);

} // namespace Nz

struct EquippableComponent : public Ndk::Component<EquippableComponent>, public Nz::HandledObject<EquippableComponent>
{
    BodyPart bodypart {};
    Side side { Side::Both };

    SkillStore::LightId attackId { SkillStore::InvalidID }; // For attack items (e.g. sword, bow)

    static inline const char* componentName() { return "equippable"; }
    static Ndk::ComponentIndex componentIndex;

private:
    friend unsigned int Nz::LuaImplQueryArg(const Nz::LuaState& state, int index, EquippableComponent* component, Nz::TypeTag<EquippableComponent>);
    friend int Nz::LuaImplReplyVal(const Nz::LuaState& state, EquippableComponentHandle&& component, Nz::TypeTag<EquippableComponentHandle>);
};

#endif // EQUIPPABLECOMPONENT_HPP
