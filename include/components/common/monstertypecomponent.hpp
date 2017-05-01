// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MONSTERTYPECOMPONENT_HPP
#define MONSTERTYPECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/String.hpp>

struct MonsterTypeComponent : public Ndk::Component<MonsterTypeComponent>
{
    ///
    /// \brief Monster's behavior
    ///        - Melee
    ///          Goes near enemy and attack it
    ///
    ///        - Stalker
    ///          Acts like Melee, or use a spell to become invisible and hide near the enemy
    ///          When invisible, puts traps on the ground
    ///          Becomes visible if attacking directly
    ///
    ///        - HitAndRun
    ///          Acts like Melee and retreat
    ///
    ///        - DSummonner (Defensive Summonner)
    ///          Summons others monsters and hide behind them
    ///          Also boost the summonned monsters, and others allies
    ///          Attack only if needed
    ///
    ///        - OSummoner (Offensive Summoner)
    ///          Summons others monsters and fight with them
    ///          Prefers to attack enemies rather than boost allies
    ///
    ///        - Summonner
    ///          Mix between Defensive Summoner and Offensive Summoner
    ///          AKA "Neutral Summoner"
    ///
    ///        - LongDistance
    ///          Attacks and hide from long distance
    ///

    Nz::String behavior;
    Nz::String family;

    static Ndk::ComponentIndex componentIndex;
};

#endif // MONSTERTYPECOMPONENT_HPP
