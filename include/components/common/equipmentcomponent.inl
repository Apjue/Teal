// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "equipmentcomponent.hpp"


Ndk::EntityHandle EquipmentComponent::get(BodyPart part, Side side)
{
    for (auto& e : equipped)
    {
        auto& equip = e->GetComponent<EquippableComponent>();

        if (equip.bodypart == part && (side == Side::Both || equip.side == side))
            return e;
    }

    return Ndk::EntityHandle {};
}

void EquipmentComponent::set(Ndk::EntityHandle equipment)
{
    auto& equipEquip = equipment->GetComponent<EquippableComponent>();
    Ndk::EntityHandle alreadySetEquipment = get(equipEquip.bodypart, equipEquip.side); // todo: what if same side but different could word?

    if (alreadySetEquipment)
        equipped.Remove(alreadySetEquipment);

    equipped.Insert(equipment);
}


bool EquipmentComponent::has(BodyPart part, Side side)
{
    return get(part, side);
}

