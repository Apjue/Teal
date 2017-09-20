// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "data/skilldata.hpp"
#include "util/assert.hpp"

SkillData::SkillData(const LuaArguments& args)
{
    TealException(args.vars.size() == 13, "Wrong number of arguments. Need 13");

    if (!args.tables.empty() && !args.tables[0]->tables.empty())
        for (auto table : args.tables[0]->tables)
        {
            if (table->vars.at(0).get<Nz::String>() == "damage")
            {
                TealException(!table->vars.empty(), "Empty arguments !");
                attackList.push_back(std::make_shared<DamageData>(*(table.get())));
            }

            else if (table->vars.at(0).get<Nz::String>() == "state")
            {
                TealException(!table->vars.empty(), "Empty arguments !");
                attackList.push_back(std::make_shared<StateData>(*(table.get())));
            }

            else if (table->vars.at(0).get<Nz::String>() == "effect")
            {
                TealException(!table->vars.empty(), "Empty arguments !");
                attackList.push_back(std::make_shared<EffectData>(*(table.get())));
            }
        }

    movementPoints = tounsigned(args.vars[0].get<double>());
    actionPoints = tounsigned(args.vars[1].get<double>());

    //effectId = static_cast<EffectStore::EffectId>(args.vars[2].get<double>());

    minRange = tounsigned(args.vars[3].get<double>());
    maxRange = tounsigned(args.vars[4].get<double>());
    modifiableRange = args.vars[5].get<bool>();

    areaType = stringToAreaType(args.vars[6].get<Nz::String>());
    areaMinRange = tounsigned(args.vars[7].get<double>());
    areaMaxRange = tounsigned(args.vars[8].get<double>());

    codename = args.vars[9].get<Nz::String>();
    name = args.vars[10].get<Nz::String>();
    description = args.vars[11].get<Nz::String>();
    icon = args.vars[12].get<Nz::String>();
}

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
