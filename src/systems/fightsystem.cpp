// Copyright (C) 2018 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "cache/doublestore.hpp"
#include "data/states.hpp"
#include "components/common/lifecomponent.hpp"
#include "components/common/fightcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/animationcomponent.hpp"
#include "components/common/blocktilecomponent.hpp"
#include "components/common/damagemodifiercomponent.hpp"
#include "components/common/equipmentcomponent.hpp"
#include "components/common/combatbehaviorcomponent.hpp"
#include "def/gamedef.hpp"
#include "def/systemdef.hpp"
#include "systems/fightsystem.hpp"

FightSystem::FightSystem()
{
    Requires<FightComponent, LifeComponent, PositionComponent>();
    SetMaximumUpdateRate(Def::MaxSystemUPS);
    SetUpdateOrder(Def::FightSystemUpdateOrder); // blocktile, animation
}

void FightSystem::OnUpdate(float)
{

}
