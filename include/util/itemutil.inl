// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <algorithm>
#include "components/items/itemcomponent.hpp"
#include "components/items/consumablecomponent.hpp"
#include "components/items/hpgaincomponent.hpp"
#include "components/items/equippablecomponent.hpp"
#include "components/common/equipmentcomponent.hpp"
#include "components/common/statecomponent.hpp"
#include "util/assert.hpp"
#include "itemutil.hpp"

bool isItemUsable(Ndk::EntityHandle item)
{
    TealAssert(item->HasComponent<ItemComponent>(), "item isn't an item?");
    return item->HasComponent<EquippableComponent>() || item->HasComponent<ConsumableComponent>();
}

bool useItem(Ndk::EntityHandle character, Ndk::EntityHandle item)
{
    TealAssert(isItemUsable(item), "Item isn't usable");

    if (item->HasComponent<EquippableComponent>())
    {
        character->GetComponent<EquipmentComponent>().set(item);
        return false;
    }

    if (item->HasComponent<ConsumableComponent>())
    {
        if (item->HasComponent<HPGainComponent>())
        {
            auto& hpGain = item->GetComponent<HPGainComponent>();
            auto& life = character->GetComponent<LifeComponent>();

            life.hp = (hpGain.rel == 0 ? hpGain.abs : unsigned(std::max(int(life.hp) + hpGain.rel, 0)));
            life.hp = std::min(life.hp, life.maxHp);
        }

        if (item->HasComponent<StateComponent>())
        {
            auto& charStates = character->GetComponent<StateComponent>().states;
            auto& itemStates = item->GetComponent<StateComponent>().states;

            charStates.reserve(itemStates.size());
            charStates.insert(charStates.end(), itemStates.begin(), itemStates.end());
        }

        return true;
    }

    throw std::runtime_error { "Unsupported type?" };
}
