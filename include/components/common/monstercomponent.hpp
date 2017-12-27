// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef MONSTERCOMPONENT_HPP
#define MONSTERCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/String.hpp>

struct MonsterComponent : public Ndk::Component<MonsterComponent>
{
    Nz::String family; // aka fight_ai_monster_family
    Nz::String name;   // aka fight_ai_monster_name

    static Ndk::ComponentIndex componentIndex;
};

#endif // MONSTERCOMPONENT_HPP
