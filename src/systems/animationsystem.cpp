// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems/animationsystem.hpp"

AnimationSystem::AnimationSystem()
{
    Requires<AnimationComponent, Ndk::GraphicsComponent, PositionComponent, OrientationComponent, RenderablesStorageComponent>();
    SetUpdateRate(Def::MAXSYSTEMUPS);
    SetUpdateOrder(5);
}

void AnimationSystem::OnUpdate(float elapsed)
{
    for (auto& e : GetEntities())
    {
        auto& anim = e->GetComponent<AnimationComponent>();
        bool  moving = e->GetComponent<PositionComponent>().moving;
        auto& gfx = e->GetComponent<Ndk::GraphicsComponent>();
        auto  dir = e->GetComponent<OrientationComponent>().dir;
        auto& sprites = e->GetComponent<RenderablesStorageComponent>().sprites;

        if (sprites.empty())
            continue;
        
        int const intDir = static_cast<int>(dir);

        unsigned const startX = intDir * anim.size.x; // Get the x and the y
        unsigned const startY = anim.frame * anim.size.y;

        switch (anim.animationState)
        {
        case AnimationComponent::OnMove:
            for (auto sprite : sprites)
                OnMoveAnimation(startX, startY, sprite, anim, moving);
            break;

        case AnimationComponent::OnEmote: // EmoteStore ?
            break;

        case AnimationComponent::OnFight:
            break;
        }
    }
}

void AnimationSystem::OnMoveAnimation(unsigned startX, unsigned startY, Nz::SpriteRef sprite,
                                      AnimationComponent& anim, bool moving)
{
    if (!moving || anim.maxframe == 0) // Reset animation, and change direction
    {
        anim.frame = 0;
        sprite->SetTextureRect({ startX, 0u, anim.size.x, anim.size.y });
        anim.animated = false;

        return;
    }

    else // Animation !
    {
        sprite->SetTextureRect({ startX, startY, anim.size.x, anim.size.y });

        ++anim.frame;
        anim.animated = true;

        if (anim.frame > anim.maxframe)
            anim.frame = 0;
    }
}
