// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include <Nazara/Core/String.hpp>
#include "def/typedef.hpp"
#include "util/util.hpp"

struct Effect
{
    Effect() = default;
    virtual ~Effect() = default;
};

struct PullEffect : public Effect
{
    PullEffect() = default;

    PullEffect(const LuaArguments& args)
    {
        TealException(args.vars.size() == 4, "Wrong number of arguments. Need 4");
        tiles = unsigned(args.vars[3].get<double>());
    }

    unsigned tiles {};

    static Nz::String getMetadataID()
    {
        return "pull";
    }
};

struct PushEffect : public Effect
{
    PushEffect() = default;

    PushEffect(const LuaArguments& args)
    {
        TealException(args.vars.size() == 4, "Wrong number of arguments. Need 4");
        tiles = unsigned(args.vars[3].get<double>());
    }

    unsigned tiles {};

    static Nz::String getMetadataID()
    {
        return "push";
    }
};

#endif // EFFECTS_HPP
