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
            }

        movementPoints = static_cast<unsigned>(args.vars[0].get<double>());
        actionPoints = static_cast<unsigned>(args.vars[1].get<double>());

        //effectId = static_cast<EffectStore::EffectId>(args.vars[2].get<double>());

        minRange = static_cast<unsigned>(args.vars[3].get<double>());
        maxRange = static_cast<unsigned>(args.vars[4].get<double>());
        modifiableRange = args.vars[5].get<bool>();

        areaType = stringToAreaType(args.vars[6].get<Nz::String>());
        areaMinRange = static_cast<unsigned>(args.vars[7].get<double>());
        areaMaxRange = static_cast<unsigned>(args.vars[8].get<double>());

        codename = args.vars[9].get<Nz::String>();
        name = args.vars[10].get<Nz::String>();
        description = args.vars[11].get<Nz::String>();
        icon = args.vars[12].get<Nz::String>();
    }

    std::vector<std::shared_ptr<Attack>> attackList;

    unsigned movementPoints { 0 }; // MP/AP required
    unsigned actionPoints { 3 };

    // EffectStore::Id effectId {}; // push, pull, etc.

    unsigned minRange { 1u };
    unsigned maxRange { 5u };
    bool modifiableRange { false };
    
    enum class AreaType
    {
        Cross,
        Plus,
        Circle,

        AlignedCenterRL, // Will switch to AlignedRight if number is uneven
        AlignedCenterDU, // Will switch to AlignedUp if number is uneven
        AlignedRight,
        AlignedLeft,
        AlignedDown,
        AlignedUp
    };

    static AreaType stringToAreaType(Nz::String string)
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
