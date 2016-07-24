// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ITEMS_HPP
#define ITEMS_HPP

#include <anax/World.hpp>
#include <anax/Component.hpp>
#include <anax/Entity.hpp>

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

namespace Items
{

///
/// \brief The HPGain struct
///
/// Adds diff HPs to the player
/// If diff == 0, the HP of the player becomes abs.
///

struct HPGain : public anax::Component
{
    int diff{};
    unsigned abs{};
};

struct AttackBonus : public anax::Component
{
    int air{};
    int fire{};
    int water{};
    int earth{};
    int neutral{};
};

struct AttackResistance : public anax::Component
{
    int air{};
    int fire{};
    int water{};
    int earth{};
    int neutral{};
};

///
/// \brief The Item struct
///
/// Contains all base informations
/// It must be in every items
///

struct Item : public anax::Component
{};

struct Stackable : public anax::Component
{
    unsigned long id;
};

struct Equippable : public anax::Component
{
    enum BodyPart
    {
        Head,  // casque
        Neck,  // amulettes, colliers
        Arms,  // épaulières, protections de bras
        Wrist, // bracelets
        Hands, // armes, boucliers
        Digit, // bagues
        Chest, // armures
        Hip,   // ceintures
        Legs,  // protections de jambes
        Feet   // bottes
    };

    enum class Side
    {
        Right,
        Left
    };
};

struct Edible : public anax::Component
{
    HPGain* hpgain{};
};

struct Resource : public anax::Component
{};

}

#endif // ITEMS_HPP
