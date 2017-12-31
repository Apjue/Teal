// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef COMBATBEHAVIORCOMPONENT_HPP
#define COMBATBEHAVIORCOMPONENT_HPP

#include <NDK/Component.hpp>

struct CombatBehaviorComponent : public Ndk::Component<CombatBehaviorComponent>
{
    ///
    /// \brief Monster's behavior
    ///        - Melee
    ///          Hunt enemy
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

    enum Behavior
    {
        Passive = 0,
        Melee,
        Stalker,
        HitAndRun,
        DSummonner,
        OSummoner,
        Summonner,
        LongDistance
    };

    Behavior behavior;

    static Ndk::ComponentIndex componentIndex;
};

#endif // COMBATBEHAVIORCOMPONENT_HPP
