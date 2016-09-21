// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems/posrefreshsystem.hpp"

PosRefreshSystem::PosRefreshSystem()
{
    Requires<PositionComponent, DefaultGraphicsPosComponent,
        Ndk::GraphicsComponent, Ndk::NodeComponent>();
}

void PosRefreshSystem::OnUpdate(float elapsed)
{
    NazaraUnused(elapsed);

    for (auto& e : GetEntities())
    {
        auto& pos = e->GetComponent<PositionComponent>();
        auto& gfxcomp = e->GetComponent<Ndk::GraphicsComponent>();
        auto& gfxpos = e->GetComponent<Ndk::NodeComponent>();
        auto& dpos = e->GetComponent<DefaultGraphicsPosComponent>();

        Nz::Vector2f defPos { dpos.x, dpos.y };

        unsigned const gX = pos.x * Def::TILEGXSIZE; // logic pos -> graphics pos
        unsigned const gY = pos.y * Def::TILEGYSIZE;
        int const gInX = pos.inX * Def::MAXGXPOSINTILE;
        int const gInY = pos.inY * Def::MAXGYPOSINTILE;

        float finalX = static_cast<float>(gX) + static_cast<float>(gInX); // We will move using this
        float finalY = static_cast<float>(gY) + static_cast<float>(gInY); // (so it's graphics pos)

        finalX += defPos.x;
        finalY += defPos.y;

        if (finalX != gfxpos.GetPosition().x  // if the entity is already at that position
         || finalY != gfxpos.GetPosition().y) // no need to move it
        {
            float const moveX = finalX - gfxpos.GetPosition().x;
            float const moveY = finalY - gfxpos.GetPosition().y;

            gfxpos.Move(moveX, moveY);
        }
    }
}
