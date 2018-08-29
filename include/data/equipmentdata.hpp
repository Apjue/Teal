// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_EQUIPMENTDATA_HPP
#define TEAL_EQUIPMENTDATA_HPP

enum class BodyPart
{
    Head,  // Casque
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

enum class Side // For digits, wrists, hands
{
    Both, // A heavy hammer or a bow need both hands
    Right,
    Left
};


inline BodyPart stringToBodypart(Nz::String string);
inline const char* bodypartToString(BodyPart bodypart);

inline Side stringToSide(Nz::String string);
inline const char* sideToString(Side side);

#include "equipmentdata.inl"

#endif // TEAL_EQUIPMENTDATA_HPP
