// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <util/nzstlcompatibility.hpp>
#include "util/assert.hpp"
#include "data/skilldata.hpp"

SkillData::AreaType SkillData::stringToAreaType(Nz::String string)
{
    string = string.ToLower();

    if (string == "cross")
        return AreaType::Cross;

    if (string == "plus")
        return AreaType::Plus;

    if (string == "circle")
        return AreaType::Circle;

    if (string == "alignedcenterrl")
        return AreaType::AlignedCenterRL;

    if (string == "alignedcenterdu")
        return AreaType::AlignedCenterDU;

    if (string == "alignedright")
        return AreaType::AlignedRight;

    if (string == "alignedleft")
        return AreaType::AlignedLeft;

    if (string == "aligneddown")
        return AreaType::AlignedDown;

    if (string == "alignedup")
        return AreaType::AlignedUp;

    throw std::runtime_error { "Invalid area type !" };
}

Nz::String SkillData::areaTypeToString(AreaType area)
{
    switch (area)
    {
        case AreaType::Cross:
            return "cross";

        case AreaType::Plus:
            return "plus";

        case AreaType::Circle:
            return "circle";

        case AreaType::AlignedCenterRL:
            return "alignedcenterrl";

        case AreaType::AlignedCenterDU:
            return "alignedcenterdu";

        case AreaType::AlignedRight:
            return "alignedright";

        case AreaType::AlignedLeft:
            return "alignedleft";

        case AreaType::AlignedDown:
            return "aligneddown";

        case AreaType::AlignedUp:
            return "alignedup";
    }

    throw std::runtime_error { "Invalid area type !" };
}

std::unordered_map<Element, unsigned> SkillData::getMaximumDamage() const // todo: redo this
{
    std::unordered_map<Element, unsigned> damage {};

    for (const std::shared_ptr<Attack>& attack : attackEffects)
    {
        if (attack->target == Attack::Target::Allies)
            continue;

        switch (attack->getAttackType())
        {
            case Attack::AttackType::Damage:
            {
                DamageData* dmg = static_cast<DamageData*>(attack.get());
                damage[dmg->damage.first] += dmg->damage.second;

                break;
            }

            case Attack::AttackType::State:
            {
                StateData* state = static_cast<StateData*>(attack.get());
                auto maxDamage = state->state->getFightInfo().maximumDamage;

                for (Element e {}; e <= Element::Max; ++e)
                    if (maxDamage[e] > 0)
                        damage[e] += maxDamage[e];

                break;
            }
        }
    }

    return damage;
}

namespace Nz
{

unsigned int LuaImplQueryArg(const LuaState& state, int index, SkillData* skill, TypeTag<SkillData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    skill->codename = state.CheckField<Nz::String>("codename", index);
    skill->displayName = state.CheckField<Nz::String>("display_name", index);
    skill->description = state.CheckField<Nz::String>("description", index);
    skill->icon = state.CheckField<Nz::String>("icon", index);

    skill->attackEffects = state.CheckField<std::vector<std::shared_ptr<Attack>>>("attacks");

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

int LuaImplReplyVal(const LuaState& state, SkillData&& skill, TypeTag<SkillData>)
{
    state.PushTable();
    {
        state.PushField("codename", skill.codename);
        state.PushField("display_name", skill.displayName);
        state.PushField("description", skill.description);
        state.PushField("icon", skill.icon);

        state.PushField("attacks", skill.attackEffects); // invalid vector afterwards ?!

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
