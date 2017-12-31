// Copyright (C) 2018 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "attackdata.hpp"
#include "def/typedef.hpp"

struct Attack
{
    Attack() = default;
    Attack(const LuaArguments& args)
    {
        if (args.vars.size() >= 2)
            data.target = AttackData::stringToTarget(args.vars[1].get<Nz::String>());
    }

    virtual ~Attack() = default;

    AttackData data;
};

#endif // ATTACK_HPP
