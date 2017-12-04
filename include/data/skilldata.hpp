// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef SKILLDATA_HPP
#define SKILLDATA_HPP

#include <Nazara/Core/String.hpp>
#include <memory>
#include <vector>
#include <utility>
#include "attack.hpp"
#include "damagedata.hpp"
#include "statedata.hpp"
#include "effectdata.hpp"
#include "def/typedef.hpp"

struct SkillData
{
    SkillData() = default;
    SkillData(const LuaArguments& args);

    enum class AttackType
    {
        Damage,
        State,
        Effect
    };

    std::vector<std::pair<AttackType, std::shared_ptr<Attack>>> attackList;

    unsigned movementPoints { 0 }; // MP/AP required
    unsigned actionPoints { 3 };

    // EffectStore::Id effectId {}; // push, pull, etc.

    unsigned minRange { 1u };
    unsigned maxRange { 5u };
    bool modifiableRange { false };
    bool viewThroughWalls { false }; // don't wallhack pls

    enum class AreaType
    {
        Cross,
        Plus,
        Circle,

        AlignedCenterRL,
        AlignedCenterDU,
        AlignedRight,
        AlignedLeft,
        AlignedDown,
        AlignedUp
    };

    static AreaType stringToAreaType(Nz::String string);
    static Nz::String areaTypeToString(AreaType area);

    AreaType areaType { AreaType::Cross };
    unsigned areaMinRange { 0u };
    unsigned areaMaxRange { 0u };

    Nz::String codename;
    Nz::String name;

    Nz::String description;
    Nz::String fightDescription;
    Nz::String icon;

    // void generateFightDescription();
};

#endif // SKILLDATA_HPP
