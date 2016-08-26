// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ITEMS_HPP
#define ITEMS_HPP

#include <NDK/World.hpp>
#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>

#include <array>
#include <vector>
#include <tuple>
#include <utility>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <micropather.h>
#include "global.hpp"
#include "util.hpp"

namespace Components
{

struct Name;
struct Level;

}

namespace Items
{

///
/// \brief The HPGain struct
///
/// Adds diff HPs to the player
/// If diff == 0, the HP of the player becomes abs.
///

struct HPGain : public Ndk::Component<HPGain>
{
    int diff{};
    unsigned abs{};

    static Ndk::ComponentIndex componentIndex;
};

struct AttackBonus : public Ndk::Component<AttackBonus>
{
    int air{};
    int fire{};
    int water{};
    int earth{};
    int neutral{};

    static Ndk::ComponentIndex componentIndex;
};

struct AttackResistance : public Ndk::Component<AttackResistance>
{
    int air{};
    int fire{};
    int water{};
    int earth{};
    int neutral{};

    static Ndk::ComponentIndex componentIndex;
};

///
/// \brief The Item struct
///
/// Contains all base informations
/// It must be in every items
///

struct Item : public Ndk::Component<Item>
{
    Components::Name* name{};
    Components::Level* level{};

    static Ndk::ComponentIndex componentIndex;
};

struct Stackable : public Ndk::Component<Stackable>
{
    unsigned long id;

    static Ndk::ComponentIndex componentIndex;
};

struct Equippable : public Ndk::Component<Equippable>
{
    enum BodyPart
    {
        Head,  // casque
        Neck,  // amulettes, colliers
        Arms,  // épaulières, protections de bras
        Hands, // armes, boucliers
        Wrist, // bracelets
        Digit, // bagues
        Chest, // armures
        Hip,   // ceintures
        Legs,  // protections de jambes
        Feet   // bottes
    };

    enum class Side //For digit, wrists, hands
    {
        Right,
        Left
    };

    BodyPart bodypart;
    Side side;

    static Ndk::ComponentIndex componentIndex;
};

struct Edible : public Ndk::Component<Edible>
{
    HPGain* hpgain {};

    static Ndk::ComponentIndex componentIndex;
};

struct Resource : public Ndk::Component<Resource>
{
    //...

    static Ndk::ComponentIndex componentIndex;
};

}

#endif // ITEMS_HPP
