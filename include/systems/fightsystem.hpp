// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef FIGHTSYSTEM_HPP
#define FIGHTSYSTEM_HPP

#include <NDK/System.hpp>
#include "def/gamedef.hpp"
#include "cache/doublestore.hpp"
#include "data/states.hpp"
#include "components/common/lifecomponent.hpp"
#include "components/common/fightcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/animationcomponent.hpp"
#include "components/common/blocktilecomponent.hpp"
#include "components/common/attackmodifiercomponent.hpp"
#include "components/common/resistancemodifiercomponent.hpp"
#include "components/common/equipmentcomponent.hpp"
#include "components/common/combatbehaviorcomponent.hpp"

class FightSystem : public Ndk::System<FightSystem>
{
public:
    FightSystem();
    ~FightSystem() = default;

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float) override;
};

#endif // FIGHTSYSTEM_HPP
