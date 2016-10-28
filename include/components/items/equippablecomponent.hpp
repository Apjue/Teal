// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EQUIPPABLECOMPONENT_HPP
#define EQUIPPABLECOMPONENT_HPP

#include <NDK/Component.hpp>

namespace Items
{

struct EquippableComponent : public Ndk::Component<EquippableComponent>
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

} // namespace Items

#endif // EQUIPPABLECOMPONENT_HPP