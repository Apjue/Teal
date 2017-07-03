// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

void InventoryComponent::add(const Ndk::EntityHandle& e)
{
    TealAssert(isItemEntity(e), "Entity isn't an item !");
    m_items.Insert(e.GetObject());
}

void InventoryComponent::remove(const Ndk::EntityHandle& e)
{
    m_items.Remove(e.GetObject());
}

bool InventoryComponent::has(const Ndk::EntityHandle& e)
{
    return m_items.Has(e.GetObject());
}

void InventoryComponent::clear()
{
    m_items.Clear();
}

const Ndk::EntityList& InventoryComponent::getAll() const
{
    return m_items;
}
