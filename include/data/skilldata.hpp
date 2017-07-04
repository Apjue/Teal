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
#include "def/typedef.hpp"

struct SkillData
{
    SkillData() = default;

    SkillData(const LuaArguments& args)
    {

    }

    std::vector<std::shared_ptr<Attack>> attackList;

    unsigned movementPoints { 0 }; // MP/AP required
    unsigned actionPoints { 3 };

    // EffectStore::ID effect {}; // push, pull, etc.

    unsigned minRange { 1u };
    unsigned maxRange { 5u };
    bool modifiableRange { false };
    
    enum class AreaType
    {
        Cross,
        Plus,
        Circle,

        AlignedCenterRL, // Will switch to AlignedUp if number is uneven
        AlignedCenterDU, // Will switch to AlignedUp if number is uneven
        AlignedRight,
        AlignedLeft,
        AlignedDown,
        AlignedUp
    };

    AreaType areaType { AreaType::Cross };
    unsigned areaMinRange { 0u };
    unsigned areaMaxRange { 0u };

    Nz::String codename;
    Nz::String name;

    Nz::String description;
    Nz::String fightDescription;
    Nz::TextureRef icon;

    // void generateFightDescription();
};

#endif // SKILLDATA_HPP
