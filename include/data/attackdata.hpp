// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ATTACKDATA_HPP
#define ATTACKDATA_HPP

#include <Nazara/Core/String.hpp>

struct AttackData
{
    enum class Target
    {
        Allies,
        Enemies,
        Both
    };

    Target target { Target::Enemies };

    static Target stringToTarget(Nz::String string)
    {
        string = string.ToLower();

        if (string == "allies")
            return Target::Allies;

        if (string == "enemies")
            return Target::Enemies;

        if (string == "both")
            return Target::Both;

        return Target::Enemies;
    }
};

#endif // ATTACKDATA_HPP
