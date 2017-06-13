// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef INVENTORYCOMPONENT_HPP
#define INVENTORYCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/World.hpp>
#include <Nazara/Core/Error.hpp>
#include <unordered_set>
#include <unordered_map>
#include "components/items/itemcomponent.hpp"
#include "components/items/equippablecomponent.hpp"
#include "components/items/ediblecomponent.hpp"
#include "components/items/resourcecomponent.hpp"
#include "util/entityutil.hpp"

///
///
/// \class InventoryComponent
///
/// \brief Contains groups of items
///
/// \note Items are entities which at least have
///       the Item Component
///

class InventoryComponent : public Ndk::Component<InventoryComponent>
{
public:
    class Group
    {
    public:
        Group() = default;
        Group(const std::string& name_) : name { name_ } {}
        ~Group() = default;

        inline void add(const Ndk::EntityHandle& e);
        inline void remove(const Ndk::EntityHandle& e);

        std::string name { "undefined" }; // ID of the group
        Ndk::EntityList entities; // entities of the group
    };

    inline InventoryComponent();
    ~InventoryComponent() = default;

    void add(const Ndk::EntityHandle& e);
    void remove(const Ndk::EntityHandle& e);
    bool has(const Ndk::EntityHandle& e);

    inline const Ndk::EntityList getAll();
    inline const Group group(const std::string& name);

    static Ndk::ComponentIndex componentIndex;

private:
    std::unordered_map<std::string, Group> m_groups;

    ///
    /// \fn reset
    ///
    /// \brief Clears the inventory
    ///        Also used to initialize the inventory with m_empty groups
    ///

    void reset();
};

#include "inventorycomponent.inl"

#endif // INVENTORYCOMPONENT_HPP
