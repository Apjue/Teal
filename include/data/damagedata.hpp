// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef DAMAGEDATA_HPP
#define DAMAGEDATA_HPP

#include <utility>
#include "elementdata.hpp"
#include "attack.hpp"
#include "def/typedef.hpp"

struct DamageData : public Attack
{
    DamageData() = default;
    DamageData(Element e, int d) : damage { e, d } {}

    DamageData(const LuaArguments& args) : Attack(args)
    {
        TealException(args.vars.size() == 4, "Wrong number of arguments. Need 4");
        TealAssert(args.vars[0].get<Nz::String>() == "damage", "Wrong type");

        damage.first = stringToElement(args.vars[2].get<Nz::String>());
        damage.second = static_cast<int>(args.vars[3].get<double>());
    }


    std::pair<Element, int> damage;
};

#endif // DAMAGEDATA_HPP
