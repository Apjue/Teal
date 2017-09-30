// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/common/positioncomponent.hpp"
#include "components/common/pathcomponent.hpp"
#include "components/common/orientationcomponent.hpp"
#include "components/common/fightcomponent.hpp"
#include "util/movementutil.hpp"
#include "cache/tilesetcore.hpp"
#include "def/systemdef.hpp"
#include "systems/movementsystem.hpp"

MovementSystem::MovementSystem()
{
    Requires<PathComponent, PositionComponent, OrientationComponent>();
    SetFixedUpdateRate(Def::MaxSystemUPS);
    SetUpdateOrder(Def::MovementSystemUpdateOrder);
}

void MovementSystem::OnUpdate(float elapsed)
{
    NazaraUnused(elapsed);

    for (auto& e : GetEntities())
    {
        if (e->GetComponent<PathComponent>().path.empty())
            continue;

        moveEntity(e, !(e->HasComponent<FightComponent>() && e->GetComponent<FightComponent>().isFighting));
    }
}
