// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EQUIPMENTCOMPONENT_HPP
#define EQUIPMENTCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/EntityList.hpp>

struct EquipmentComponent : public Ndk::Component<EquipmentComponent>
{
    Ndk::EntityList equipped;

    static Ndk::ComponentIndex componentIndex;
};

#endif // EQUIPMENTCOMPONENT_HPP
