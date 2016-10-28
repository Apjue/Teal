// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

void InventoryComponent::Group::add(const EntityType& e)
{
    if (entities.find(e) != entities.end())
        entities.insert(e);
}

void InventoryComponent::Group::remove(const EntityType& e)
{
    auto it = entities.find(e);

    if (it != entities.end())
        entities.erase(it);
}


InventoryComponent::InventoryComponent(Ndk::WorldHandle& world) : m_world(world)
{
    reset();
}

const InventoryComponent::EntityCache& InventoryComponent::getAll()
{
    return m_groups["all"].entities;
}

const InventoryComponent::Group& InventoryComponent::group(const std::string& name)
{
    return m_groups[name];
}

void InventoryComponent::assertItem(const Ndk::EntityHandle& entity) const
{
    NazaraAssert(entity.IsValid(), "Handle isn't even valid !");
    NazaraAssert(entity->IsValid(), "Entity isn't valid !");
    NazaraAssert(entity->HasComponent<Items::ItemComponent>(), "Entity isn't an item !");
}
