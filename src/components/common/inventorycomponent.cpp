// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/common/inventorycomponent.hpp"

void InventoryComponent::add(const EntityType& e)
{
    assertItem(e);

    m_groups["all"].add(e);

    //Add to basic groups
    if (e->HasComponent<Items::EdibleComponent>())
        m_groups["edible"].add(e);

    if (e->HasComponent<Items::EquippableComponent>())
        m_groups["equippable"].add(e);

    if (e->HasComponent<Items::ResourceComponent>())
        m_groups["resource"].add(e);
}

void InventoryComponent::remove(const EntityType& e)
{
    assertItem(e);

    for (auto& group : m_groups)
    {
        group.second.remove(e);
    }
}

bool InventoryComponent::has(const EntityType& e)
{
    assertItem(e);

    auto& group = m_groups["all"];
    auto it = group.entities.find(e);

    return (it == group.entities.end());
}

void InventoryComponent::reset()
{
    for (auto const& name : { "all", "edible", "equippable", "resource" })
    {
        m_groups[name] = Group { name };
    }
}
