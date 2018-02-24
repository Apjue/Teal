// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EQUIPPABLECOMPONENT_HPP
#define EQUIPPABLECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Lua/LuaState.hpp>
#include "cache/doublestore.hpp"
#include "def/typedef.hpp"

extern void initializeEquippableComponent(const SkillStore* skillstore_);
struct EquippableComponent;

namespace Nz
{

extern unsigned int LuaImplQueryArg(const LuaState& state, int index, EquippableComponent* component, TypeTag<EquippableComponent>);
extern int LuaImplReplyVal(const LuaState& state, EquippableComponent&& component, TypeTag<EquippableComponent>);

} // namespace Nz

struct EquippableComponent : public Ndk::Component<EquippableComponent>
{
    enum BodyPart
    {
        Head,  // Casque
        Neck,  // Amulette
        Arms,  // Protections de bras
        Hands, // Armes / boucliers
        Wrists, // Bracelets
        Digits, // Bagues
        Chest, // armure
        Hip,   // Ceinture
        Legs,  // Jeans
        Feet   // Bottes
    } bodypart {};

    enum Side // For digits, wrists, hands
    {
        Both, // A heavy hammer or a bow need both hands
        Right,
        Left
    } side {};

    SkillStore::LightId attackId { SkillStore::InvalidID }; // For attack items (e.g. sword, bow)


    static BodyPart stringToBodypart(Nz::String string);
    static const char* bodypartToString(BodyPart bodypart);
    static Side stringToSide(Nz::String string);
    static const char* sideToString(Side side);

    static inline const char* componentName() { return "equippable"; }
    static Ndk::ComponentIndex componentIndex;

private:
    static const SkillStore* skillstore;

    friend void initializeEquippableComponent(const SkillStore* skillstore);
    friend unsigned int Nz::LuaImplQueryArg(const Nz::LuaState& state, int index, EquippableComponent* component, Nz::TypeTag<EquippableComponent>);
    friend int Nz::LuaImplReplyVal(const Nz::LuaState& state, EquippableComponent&& component, Nz::TypeTag<EquippableComponent>);
};

#endif // EQUIPPABLECOMPONENT_HPP
