// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "equipmentcomponent.hpp"

const Ndk::EntityList& EquipmentComponent::getAll() const
{
    return equipped;
}

Ndk::EntityHandle EquipmentComponent::get(BodyPart part, unsigned skip)
{
    for (auto& e : equipped)
    {
        auto& equip = e->GetComponent<EquippableComponent>();

        if (equip.bodypart == part)
        {
            if (skip == 0)
                return e;

            else
                --skip;
        }
    }

    return Ndk::EntityHandle {};
}

void EquipmentComponent::set(Ndk::EntityHandle item)
{
    // Already equipped that item, remove it
    if (equipped.Has(item))
    {
        equipped.Remove(item);
        return;
    }

    // Else, equip it
    auto& equip = item->GetComponent<EquippableComponent>();

    if (equip.useBothSides)
    {
        while (get(equip.bodypart))
            equipped.Remove(get(equip.bodypart));
    }

    else
    {
        switch (has(equip.bodypart)) // Number of equipped items
        {
            case 0:
                break;

            case 1:
            {
                Ndk::EntityHandle alreadyEquipped = get(equip.bodypart);

                if (alreadyEquipped->GetComponent<EquippableComponent>().useBothSides)
                    equipped.Remove(alreadyEquipped);

                break;
            }

            case 2:
                equipped.Remove(get(equip.bodypart));
                break;

            default:
                throw std::runtime_error { "A third side ?!" };
        }
    }

    equipped.Insert(item);
}


unsigned EquipmentComponent::has(BodyPart part)
{
    unsigned counter {};

    for (auto& e : equipped)
    {
        auto& equip = e->GetComponent<EquippableComponent>();

        if (equip.bodypart == part)
            ++counter;
    }

    return counter;
}

