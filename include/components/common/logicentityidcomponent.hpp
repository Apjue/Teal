// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LOGICENTITYIDCOMPONENT_HPP
#define LOGICENTITYIDCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>
#include "def/layerdef.hpp"

///
/// \struct LogicEntityIdComponent
///
/// \brief Used by any function or class requiring to have a sprite using data from a logic entity
///
/// \example Items on the ground, items equipped by character (todo), items in inventory GUI (todo)
///

struct LogicEntityIdComponent : public Ndk::Component<LogicEntityIdComponent>
{
    enum GraphicalItemType
    {
        GroundItem,
        EquippedItem,
        InventoryItem
    };

    Ndk::EntityHandle logicEntity;
    GraphicalItemType itemType;

    int getRenderOrder() const
    {
        return getRenderOrder(itemType);
    }

    static int getRenderOrder(GraphicalItemType type)
    {
        switch (type)
        {
            case LogicEntityIdComponent::GroundItem:
                return Def::MapItemsLayer;

            case LogicEntityIdComponent::EquippedItem:
                return Def::EquippedItemsLayer;

            case LogicEntityIdComponent::InventoryItem:
                return Def::InventoryItemsLayer;

            default:
                throw std::runtime_error { "Undefined item type" };
        }
    }

    static Ndk::ComponentIndex componentIndex;
};

#endif // LOGICENTITYIDCOMPONENT_HPP
