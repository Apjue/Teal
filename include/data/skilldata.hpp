// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef SKILLDATA_HPP
#define SKILLDATA_HPP

#include <vector>
#include <utility>

struct SkillData
{
    std::vector<std::pair<unsigned /* element */, int /* damage */>> damageList;
    // 0 = neutral, 1 = air, 2 = fire, 3 = water, 4 = earth
    
    unsigned minRange {};
    unsigned maxRange {};

    // StateStore::ID stateToEnter {};
    // EffectStore::ID effect {}; // push, pull, etc.

    enum class Target
    {
        Allies,
        Enemies,
        Both
    };

    Target target {};

    enum class AreaType
    {
        Cross,
        Plus,
        Aligned,
        Perpendicular,
        Circle
    };

    AreaType areaType {};
    unsigned areaMinRange {};
    unsigned areaMaxRange {};
};

#endif // SKILLDATA_HPP
