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
    using EntityType = Ndk::EntityHandle;
    using EntityCache = std::unordered_set<EntityType>;

public:
    class Group
    {
    public:
        Group() = default;
        Group(const std::string& name_) : name { name_ } {}
        ~Group() = default;

        inline void add(const EntityType& e);
        inline void remove(const EntityType& e);

        std::string name { "undefined" }; // ID of the group
        EntityCache entities; // entities of the group
    };

    inline InventoryComponent(Ndk::WorldHandle& world);
    ~InventoryComponent() = default;

    ///
    /// \fn add
    ///
    /// \brief Adds an entity to the inventory
    ///
    /// \param e Entity to add
    ///

    void add(const EntityType& e);

    ///
    /// \fn remove
    ///
    /// \brief Removes an entity to the inventory
    ///
    /// \param e Entity to remove
    ///

    void remove(const EntityType& e);

    ///
    /// \fn has
    ///
    /// \brief Check if the inventory contains an entity
    ///
    /// \param e Entity to check
    ///
    /// \returns If the inventory contains the entity
    ///

    bool has(const EntityType& e);

    ///
    /// \fn getAll
    ///
    /// \returns All entities the inventory contains
    ///

    inline const EntityCache& getAll();

    ///
    /// \fn group
    ///
    /// \param name Name of the group
    ///
    /// \returns A group of the inventory
    ///

    inline const Group& group(const std::string& name);

    static Ndk::ComponentIndex componentIndex;

private:
    Ndk::WorldHandle m_world;
    std::unordered_map<std::string, Group> m_groups;

    ///
    /// \fn reset
    ///
    /// \brief Clears the inventory
    ///        Also used to init the inventory with empty groups
    ///

    void reset();

    ///
    /// \fn assertItem
    ///
    /// \brief Ensures the entity is an item
    ///
    /// \param entity Entity to check
    ///

    inline void assertItem(const Ndk::EntityHandle& entity) const;
};

#endif // INVENTORYCOMPONENT_HPP

#include "inventorycomponent.inl"
