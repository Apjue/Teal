// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATEDATA_HPP
#define STATEDATA_HPP

#include <memory>
#include "util/assert.hpp"
#include "attack.hpp"
#include "states.hpp"
#include "def/typedef.hpp"

struct StateData : public Attack
{
    StateData() = default;
    StateData(const LuaArguments& args) : Attack(args)
    {
        TealException(args.vars.size() >= 4, "Wrong number of arguments. Need at least 4");
        TealAssert(args.vars[0].get<Nz::String>() == "state", "Wrong type of attack");

        stateType = args.vars[3].get<Nz::String>().ToLower(); // If some state changed
                                                              // Don't forget to update this, but also
        if (stateType == PoisonnedState::getMetadataID())     // AISystem::serializeSkills (aisystem.cpp)
            state = std::make_shared<PoisonnedState>(args);

        if (stateType == HealedState::getMetadataID())
            state = std::make_shared<HealedState>(args);

        if (stateType == WeaknessState::getMetadataID())
            state = std::make_shared<WeaknessState>(args);

        if (stateType == PowerState::getMetadataID())
            state = std::make_shared<PowerState>(args);

        if (stateType == ParalyzedState::getMetadataID())
            state = std::make_shared<ParalyzedState>(args);

        if (stateType == SleepingState::getMetadataID())
            state = std::make_shared<SleepingState>(args);

        if (stateType == ConfusedState::getMetadataID())
            state = std::make_shared<ConfusedState>(args);
    }

    Nz::String stateType;
    std::shared_ptr<State> state;
};

#endif // STATEDATA_HPP
