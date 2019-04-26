// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef INVENTORYCOMPONENT_HPP
#define INVENTORYCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/EntityList.hpp>

#include <Nazara/Core/Signal.hpp>

class InventoryComponent : public Ndk::Component<InventoryComponent>
{
public: // Temporary, until I make a real inventory GUI
    InventoryComponent() = default;
    InventoryComponent(const InventoryComponent& i) : items(i.items) {}
    InventoryComponent(InventoryComponent&&) = default;
    ~InventoryComponent() = default;

    InventoryComponent& operator=(const InventoryComponent&) = default;
    InventoryComponent& operator=(InventoryComponent&&) = default;

    inline void clear()
    {
        items.Clear();
    }

    inline bool has(const Ndk::Entity* entity) const
    {
        return items.Has(entity);
    }


    inline void insert(Ndk::Entity* entity)
    {
        items.Insert(entity);
        onItemAdded(entity->CreateHandle());
    }

    inline void remove(Ndk::Entity* entity)
    {
        items.Remove(entity);
        onItemRemoved(entity->CreateHandle());
    }


    inline void reserve(std::size_t entityCount)
    {
        items.Reserve(entityCount);
    }

    inline const Ndk::EntityList& getItems() const
    {
        return items;
    }

    NazaraSignal(onItemAdded, Ndk::EntityHandle /*item*/);
    NazaraSignal(onItemRemoved, Ndk::EntityHandle /*item*/);

    static Ndk::ComponentIndex componentIndex;

private:
    Ndk::EntityList items;
};

#endif // INVENTORYCOMPONENT_HPP
