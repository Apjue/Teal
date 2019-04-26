// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef SKILLDATA_HPP
#define SKILLDATA_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <memory>
#include <vector>
#include <utility>
#include <unordered_map>
#include "data/attack.hpp"
#include "data/damagedata.hpp"
#include "data/statedata.hpp"
#include "data/effectdata.hpp"
#include "def/typedef.hpp"

struct SkillData
{
    std::vector<std::shared_ptr<Attack>> attackEffects;

    unsigned movementPoints { 0 }; // Movement Points required to use the skill
    unsigned actionPoints { 3 };   // Action Points required to use the skill

    unsigned minRange { 1u };
    unsigned maxRange { 1u };
    bool modifiableRange { false };
    bool viewThroughObstacles { false }; // don't wallhack pls

    enum class AreaType
    {
        Cross,
        Plus,
        Circle,

        AlignedCenterRL, // right & left
        AlignedCenterDU, // down & up
        AlignedRight,
        AlignedLeft,
        AlignedDown,
        AlignedUp
    };

    static AreaType stringToAreaType(Nz::String string);
    static Nz::String areaTypeToString(AreaType area);

    AreaType areaType { AreaType::Cross }; // Example: https://i.imgur.com/ycE4Fc3.png with areaType = Cross,     areaMinRange = 1, areaMaxRange = 3
    unsigned areaMinRange { 0u };  // Another Example: https://i.imgur.com/Vbo44YA.png with areaType = AlignedUp, areaMinRange = 2, areaMaxRange = 6
    unsigned areaMaxRange { 0u };

    Nz::String codename;
    Nz::String displayName;

    Nz::String description;
    Nz::TextureRef icon;

    std::unordered_map<Element, unsigned> getMaximumDamage() const;
    // FightStats generateFightStats();
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

extern unsigned int LuaImplQueryArg(const LuaState& state, int index, SkillData* skill, TypeTag<SkillData>);
extern int LuaImplReplyVal(const LuaState& state, SkillData&& skill, TypeTag<SkillData>);

} // namespace Nz

#endif // SKILLDATA_HPP
