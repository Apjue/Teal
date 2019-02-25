// Copyright (C) 2019 Samy Bensaid
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
    inline const Ndk::EntityList& getAll() const;
    inline Ndk::EntityHandle get(BodyPart part, unsigned skip = 0); // 'skip' is the number of items to... skip
    inline void set(Ndk::EntityHandle item);

    inline unsigned has(BodyPart part);


    static Ndk::ComponentIndex componentIndex;

private:
    Ndk::EntityList equipped;
    // std::unordered_map<Bodypart, std::array<std::optional<bool>, 2u>> ?
    // tsl::hopscotch_map<X, chobo::static_vector<Y, 3>> ?
};

#include "equipmentcomponent.inl"

#endif // EQUIPMENTCOMPONENT_HPP
