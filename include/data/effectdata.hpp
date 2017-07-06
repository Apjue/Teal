// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EFFECTDATA_HPP
#define EFFECTDATA_HPP

#include <memory>
#include "util/assert.hpp"
#include "attack.hpp"
#include "effects.hpp"
#include "def/typedef.hpp"

struct EffectData : public Attack
{
    EffectData() = default;
    EffectData(const LuaArguments& args) : Attack(args)
    {
        TealException(args.vars.size() >= 3, "Wrong number of arguments. Need at least 3");
        TealAssert(args.vars[0].get<Nz::String>() == "effect", "Wrong type of attack");

        Nz::String effectType = args.vars[2].get<Nz::String>().ToLower();

        if (effectType == PullEffect::getMetadataID())
            effect = std::make_shared<PullEffect>(args);

        if (effectType == PushEffect::getMetadataID())
            effect = std::make_shared<PushEffect>(args);
    }

    std::shared_ptr<Effect> effect;
};

#endif // EFFECTDATA_HPP
