// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EQUIPPABLECOMPONENT_HPP
#define EQUIPPABLECOMPONENT_HPP

#include <NDK/Component.hpp>
#include "cache/doublestore.hpp"
#include "def/typedef.hpp"

namespace Items
{

struct EquippableComponent : public Ndk::Component<EquippableComponent>
{
    EquippableComponent(const LuaArguments& args)
    {
        if (args.empty())
            return;

        TealException(args.size() == 1, "Too much arguments");
        TealException(args[0].args.size() <= 3, "Too much arguments");

        if (args[0].args.size() > 0)
            bodypart = stringToBodypart(args[0].args[0].get<Nz::String>());

        if (args[0].args.size() > 1)
            side = stringToSide(args[0].args[1].get<Nz::String>());

        if (args[0].args.size() > 2)
            attackId = static_cast<SkillStore::LightId>(args[0].args[2].get<double>());
    }

    enum BodyPart
    {
        Head = 1,  // Casque
        Neck,  // Amulette
        Arms,  // Protections de bras
        Hands, // Armes / boucliers
        Wrists, // Bracelets
        Digits, // Bagues
        Chest, // armure
        Hip,   // Ceinture
        Legs,  // Jeans
        Feet   // Bottes
    };

    BodyPart stringToBodypart(Nz::String string)
    {
        string = string.ToLower();

        if (string == "head")
            return Head;

        if (string == "neck")
            return Neck;

        if (string == "arms")
            return Arms;

        if (string == "hands")
            return Hands;

        if (string == "wrists")
            return Wrists;

        if (string == "digits")
            return Digits;

        if (string == "chest")
            return Chest;

        if (string == "hip")
            return Hip;

        if (string == "legs")
            return Legs;

        if (string == "feet")
            return Feet;

        return {};
    }

    enum Side // For digits, wrists, hands
    {
        Both = 1, // A heavy hammer or a bow need both hands
        Right,
        Left
    };

    Side stringToSide(Nz::String string)
    {
        string = string.ToLower();

        if (string == "both")
            return Both;

        if (string == "right")
            return Right;

        if (string == "left")
            return Left;

        return {};
    }

    BodyPart bodypart {};
    Side side {};

    SkillStore::LightId attackId {}; // For swords, bows, hammers...

    static Ndk::ComponentIndex componentIndex;
};

} // namespace Items

#endif // EQUIPPABLECOMPONENT_HPP
