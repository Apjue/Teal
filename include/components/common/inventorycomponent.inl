// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

InventoryComponent::InventoryComponent()
{
    reset();
}

void InventoryComponent::Group::add(const EntityType& e)
{
    auto it = entities.find(e);

    if (it == entities.end())
        entities.insert(e);
}

void InventoryComponent::Group::remove(const EntityType& e)
{
    auto it = entities.find(e);

    if (it != entities.end())
        entities.erase(it);
}

const InventoryComponent::EntityCache& InventoryComponent::getAll()
{
    return m_groups["all"].entities;
}

const InventoryComponent::Group& InventoryComponent::group(const std::string& name)
{
    return m_groups[name];
}
