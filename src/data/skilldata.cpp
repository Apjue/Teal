// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "data/skilldata.hpp"
#include "util/assert.hpp"

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

    return AreaType::Cross;
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

    return "";
}

std::unordered_map<Element, unsigned> SkillData::getMaximumDamage() const
{
    std::unordered_map<Element, unsigned> damage {};

    for (const std::shared_ptr<Attack>& attack : attackList)
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
                auto& maxDamage = state->state->getFightInfo().maximumDamage;

                for (Element e {}; e <= Element::Max; ++e)
                    if (maxDamage[e] > 0)
                        damage[e] += maxDamage[e];

                break;
            }
        }
    }

    return damage;
}
