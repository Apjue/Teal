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

struct InventoryComponent : public Ndk::Component<InventoryComponent>
{
    Ndk::EntityList items;

    static Ndk::ComponentIndex componentIndex;
};

#endif // INVENTORYCOMPONENT_HPP
