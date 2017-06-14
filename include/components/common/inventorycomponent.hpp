// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef INVENTORYCOMPONENT_HPP
#define INVENTORYCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/World.hpp>
#include <NDK/EntityList.hpp>
#include <Nazara/Core/Error.hpp>
#include "components/items/itemcomponent.hpp"
#include "util/assert.hpp"

#ifdef TEAL_DEBUG
    #include "util/entityutil.hpp" // isItemEntity
#endif

///
///
/// \class InventoryComponent
///
/// \brief Contains items
///
/// \note Items are entities which at least have the Item Component
///

class InventoryComponent : public Ndk::Component<InventoryComponent>
{
public:

    InventoryComponent() = default;
    ~InventoryComponent() = default;

    inline void add(const Ndk::EntityHandle& e);
    inline void remove(const Ndk::EntityHandle& e);
    inline bool has(const Ndk::EntityHandle& e);

    inline void clear();
    inline const Ndk::EntityList& getAll() const;

    static Ndk::ComponentIndex componentIndex;

private:
    Ndk::EntityList m_items;
};

#include "inventorycomponent.inl"

#endif // INVENTORYCOMPONENT_HPP
