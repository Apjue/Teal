// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EQUIPMENTCOMPONENT_HPP
#define EQUIPMENTCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/EntityList.hpp>
#include "components/items/equippablecomponent.hpp"
#include "data/equipmentdata.hpp"

struct EquipmentComponent : public Ndk::Component<EquipmentComponent>
{
    inline Ndk::EntityHandle get(BodyPart part, Side side = Side::Both);
    inline void set(Ndk::EntityHandle equipment);

    inline bool has(BodyPart part, Side side = Side::Both);


    static Ndk::ComponentIndex componentIndex;

private:
    Ndk::EntityList equipped;
};

#include "equipmentcomponent.inl"

#endif // EQUIPMENTCOMPONENT_HPP
