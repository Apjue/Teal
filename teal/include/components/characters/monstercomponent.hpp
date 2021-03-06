// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef MONSTERCOMPONENT_HPP
#define MONSTERCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>
#include <Nazara/Core/String.hpp>

struct MonsterComponent : public Ndk::Component<MonsterComponent>
{
    // for lua's fight_ai_monster_name, use NameComponent
    Nz::String family; // lua: fight_ai_monster_family
    Ndk::EntityHandle monsterGroupEntity;

    static Ndk::ComponentIndex componentIndex;
};

#endif // MONSTERCOMPONENT_HPP
