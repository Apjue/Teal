// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef SKILLDATA_HPP
#define SKILLDATA_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <memory>
#include <vector>
#include "attack.hpp"
#include "damagedata.hpp"
#include "statedata.hpp"
#include "characteristicmodifier.hpp"

struct SkillData
{
    std::vector<std::shared_ptr<Attack>> attackList;

    unsigned movementPoints {};
    unsigned actionPoints { 3 };

    // EffectStore::ID effect {}; // push, pull, etc.

    unsigned minRange { 1u };
    unsigned maxRange { 5u };
    
    enum class AreaType
    {
        Cross,
        Plus,
        Aligned,
        Perpendicular,
        Circle
    };

    AreaType areaType { AreaType::Cross };
    unsigned areaMinRange { 0u };
    unsigned areaMaxRange { 0u };

    Nz::String name;
    Nz::String description; // Text description. Fight description will be auto generated
    Nz::TextureRef icon;
};

#endif // SKILLDATA_HPP
