// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_MONSTERGROUPCOMPONENT_HPP
#define TEAL_MONSTERGROUPCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/EntityList.hpp>

struct MonsterGroupComponent : public Ndk::Component<MonsterGroupComponent>
{
    Ndk::EntityList monsters;

    static Ndk::ComponentIndex componentIndex;
};

#endif // TEAL_MONSTERGROUPCOMPONENT_HPP
