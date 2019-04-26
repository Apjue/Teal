// Copyright (C) 2019 Samy Bensaid
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
    Chest, // Armure
    Hip,   // Ceinture
    Legs,  // Jeans
    Feet   // Bottes
};


inline BodyPart stringToBodypart(Nz::String string);
inline const char* bodypartToString(BodyPart bodypart);

#include "equipmentdata.inl"

#endif // TEAL_EQUIPMENTDATA_HPP
