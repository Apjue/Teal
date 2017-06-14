// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef SKILLDATA_HPP
#define SKILLDATA_HPP

#include <vector>
#include <utility>
#include "elementdata.hpp"

struct SkillData
{
    std::vector<std::pair<Element, int>> damageList;
    
    unsigned minRange { 1u };
    unsigned maxRange { 5u };

    // StateStore::ID stateToEnter {};
    // EffectStore::ID effect {}; // push, pull, etc.

    enum class Target
    {
        Allies,
        Enemies,
        Both
    };

    Target target { Target::Enemies };

    enum class AreaType
    {
        Cross,
        Plus,
        Aligned,
        Perpendicular,
        Circle
    };

    AreaType areaType { AreaType::Cross };
    unsigned areaMinRange { 0u };
    unsigned areaMaxRange { 0u };
};

#endif // SKILLDATA_HPP
