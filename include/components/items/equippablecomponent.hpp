// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EQUIPPABLECOMPONENT_HPP
#define EQUIPPABLECOMPONENT_HPP

#include <NDK/Component.hpp>
#include "cache/doublestore.hpp"

namespace Items
{

struct EquippableComponent : public Ndk::Component<EquippableComponent>
{
    enum BodyPart
    {
        Head,  // Casque
        Neck,  // Amulette
        Arms,  // Protections de bras
        Hands, // Armes / boucliers
        Wrist, // Bracelets
        Digit, // Bagues
        Chest, // armure
        Hip,   // Ceinture
        Legs,  // Jeans
        Feet   // Bottes
    };

    enum class Side // For digits, wrists, hands
    {
        Right,
        Left,
        Both // A heavy hammer or a bow need both hands
    };

    BodyPart bodypart;
    Side side;

    SkillStore::LightId attackId {}; // For swords, bows, hammers...

    static Ndk::ComponentIndex componentIndex;
};

} // namespace Items

#endif // EQUIPPABLECOMPONENT_HPP
