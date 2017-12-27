// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef FIGHTCOMPONENT_HPP
#define FIGHTCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <vector>
#include "components/common/pathcomponent.hpp"
#include "cache/doublestore.hpp"
#include "global.hpp"
#include "data/states.hpp"
#include "util/nzstlcompatibility.hpp"

struct FightComponent : public Ndk::Component<FightComponent>
{
    FightComponent(bool autoAttack = false, unsigned mov = 3, unsigned action = 6)
        : automaticallyAttack { autoAttack }, maxMovementPoints { mov }, maxActionPoints { action }, movementPoints { mov }, actionPoints { action } {}

    bool isFighting { false };
    bool myTurn { false }; // Waiting to kill you
    unsigned teamNumber {};
    std::vector<std::shared_ptr<State>> states;

    bool automaticallyAttack { false }; // In the map

    unsigned maxMovementPoints { 3 }; // todo: add max movement/action points
    unsigned maxActionPoints { 6 };

    unsigned movementPoints {};
    unsigned actionPoints {};

    std::vector<SkillStore::LightId> attacks;

    SkillStore::LightId wantedAttack { SkillStore::InvalidID };
    Ndk::EntityHandle target;

    static Ndk::ComponentIndex componentIndex;
};

#endif // FIGHTCOMPONENT_HPP
