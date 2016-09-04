// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems/animationsystem.hpp"

void AnimationSystem::OnUpdate(float elapsed)
{
    for (auto& e : GetEntities())
    {
        auto& anim = e->GetComponent<AnimationComponent>();
        auto& moving = e->GetComponent<PositionComponent>().moving;
        auto& sprite = e->GetComponent<Ndk::GraphicsComponent>();
        auto& dir = e->GetComponent<OrientationComponent>().dir;

        int const intDir = static_cast<int>(dir);

        Nz::Sprite* gfx = getSpriteFromComponent(sprite);

        if (!gfx)
            continue; //No sprite has been found

        int const startX = intDir * anim.size.x; //Get the x and the y
        int const startY = anim.frame * anim.size.y;

        switch (anim.animationState)
        {
        case AnimationComponent::Undefined:
            UndefinedStateAnimation();
            break;

        case AnimationComponent::Moving:
            MovingStateAnimation(startX, startY, gfx, anim, moving);
            break;
        }
    }
}

void AnimationSystem::UndefinedStateAnimation()
{
    Nz::StringStream errLog;
    errLog << "UndefinedStateAnimation has been used";

    NazaraWarning(errLog.ToString());
}

void AnimationSystem::MovingStateAnimation(unsigned startX, unsigned startY, Nz::SpriteRef gfx,
                                           AnimationComponent& anim, bool moving)
{
    if (!moving) //No animation if not moving
    {
        if (anim.frame != 0)
        {
            anim.frame = 0;
            gfx->SetTextureRect({ startX, 0u, anim.size.x, anim.size.y });
        }
        return;
    }

    if (anim.maxframe == 0) //Only change the direction, no animation
    {
        gfx->SetTextureRect({ startX, 0, anim.size.x, anim.size.y });
    }
    else //Animation !
    {
        gfx->SetTextureRect({ startX, startY, anim.size.x, anim.size.y });

        ++anim.frame;
        if (anim.frame > anim.maxframe)
            anim.frame = 0;
    }
}
