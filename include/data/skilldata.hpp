// Copyright (C) 2018 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef SKILLDATA_HPP
#define SKILLDATA_HPP

#include <Nazara/Core/String.hpp>
#include <memory>
#include <vector>
#include <utility>
#include <unordered_map>
#include "attack.hpp"
#include "damagedata.hpp"
#include "statedata.hpp"
#include "effectdata.hpp"
#include "elementdata.hpp"
#include "def/typedef.hpp"

struct SkillData
{
    std::vector<std::shared_ptr<Attack>> attackList;

    unsigned movementPoints { 0 }; // MP/AP required
    unsigned actionPoints { 3 };

    // EffectStore::Id effectId {}; // push, pull, etc.

    unsigned minRange { 1u };
    unsigned maxRange { 1u };
    bool modifiableRange { false };
    bool viewThroughObstacles { false }; // don't wallhack pls

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
    Nz::String displayName;

    Nz::String description;
    Nz::String icon;

    std::unordered_map<Element, unsigned> getMaximumDamage() const;
    // FightStats generateFightStats();
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, SkillData* skill, TypeTag<SkillData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    skill->codename = state.CheckField<Nz::String>("codename", index);
    skill->displayName = state.CheckField<Nz::String>("displayName", index);
    skill->description = state.CheckField<Nz::String>("description", index);
    skill->icon = state.CheckField<Nz::String>("icon", index);

    state.GetField("attacks", index);
    {
        for (long long i {};; ++i)
        {
            state.PushInteger(i);

            if (state.GetTable(index) == Nz::LuaType_Table)
            {
                int index { -1 };

                skill->attackList.push_back(state.Check<std::shared_ptr<Attack>>(&index));
                state.Pop();
            }

            else
            {
                state.Pop();
                break;
            }
        }
    }

    state.Pop();

    skill->movementPoints = state.CheckField<unsigned>("movement_points", index);
    skill->actionPoints = state.CheckField<unsigned>("action_points", index);

    skill->minRange = state.CheckField<unsigned>("min_range", index);
    skill->maxRange = state.CheckField<unsigned>("max_range", index);
    skill->modifiableRange = state.CheckField<bool>("modifiable_range", index);
    skill->viewThroughObstacles = state.CheckField<bool>("view_through_walls", index);

    skill->areaType = SkillData::stringToAreaType(state.CheckField<Nz::String>("area_type", index));
    skill->areaMinRange = state.CheckField<unsigned>("area_min_range", index);
    skill->areaMaxRange = state.CheckField<unsigned>("area_max_range", index);

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, SkillData&& skill, TypeTag<SkillData>)
{
    state.PushTable();
    {
        state.PushField("codename", skill.codename);
        state.PushField("display_name", skill.displayName);
        state.PushField("description", skill.description);
        state.PushField("icon", skill.icon);

        state.PushTable();
        {
            for (unsigned i {}; i < skill.attackList.size(); ++i)
            {
                state.PushInteger(i + 1);
                state.PushTable();
                {
                    state.Push(skill.attackList[i]);
                }

                state.SetTable();
            }
        }

        state.SetField("attacks");

        state.PushField("movement_points", skill.movementPoints);
        state.PushField("action_points", skill.actionPoints);

        state.PushField("min_range", skill.minRange);
        state.PushField("max_range", skill.maxRange);
        state.PushField("modifiable_range", skill.modifiableRange);
        state.PushField("view_through_walls", skill.viewThroughObstacles);

        state.PushField("area_type", SkillData::areaTypeToString(skill.areaType));
        state.PushField("area_min_range", skill.areaMinRange);
        state.PushField("area_max_range", skill.areaMaxRange);
    }

    return 1;
}

} // namespace Nz

#endif // SKILLDATA_HPP
