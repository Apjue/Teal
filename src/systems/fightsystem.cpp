// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems/fightsystem.hpp"

FightSystem::FightSystem()
{
    Requires<FightComponent, LifeComponent, PositionComponent>();
    SetUpdateRate(Def::MAXSYSTEMUPS);
    SetUpdateOrder(3); //blocktile, animation
}

void FightSystem::OnUpdate(float)
{

}
