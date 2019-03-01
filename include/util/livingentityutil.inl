// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/characters/equipmentcomponent.hpp"
#include "util/assert.hpp"
#include "livingentityutil.hpp"

DamageModifierList getLivingEntityDamageModifier(const Ndk::EntityHandle& e)
{
    TealAssert(e->HasComponent<DamageModifierComponent>(), "Entity doesn't have DamageModifierComponent");

    DamageModifierList data = e->GetComponent<DamageModifierComponent>().data;
    auto& equip = e->GetComponent<EquipmentComponent>();

    for (auto& item : equip.getAll())
    {
        if (item->HasComponent<DamageModifierComponent>())
        {
            auto& itemData = item->GetComponent<DamageModifierComponent>().data;

            for (Element e {}; e <= Element::Max; ++e)
            {
                data[e].attack += itemData[e].attack;
                data[e].resistance += itemData[e].resistance;
            }
        }
    }

    return data;
}
