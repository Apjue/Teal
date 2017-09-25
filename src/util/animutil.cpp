// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components/GraphicsComponent.hpp>
#include "components/common/orientationcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/renderablesstoragecomponent.hpp"
#include "def/gamedef.hpp"
#include "def/systemdef.hpp"
#include "util/gfxutil.hpp"
#include "util/assert.hpp"
#include "util/entityutil.hpp"
#include "util/animutil.hpp"

bool hasRightComponentsToAnimate(const Ndk::EntityHandle& e)
{
    return e->HasComponent<AnimationComponent>() &&
        e->HasComponent<Ndk::GraphicsComponent>() &&
        e->HasComponent<PositionComponent>() &&
        e->HasComponent<OrientationComponent>() &&
        e->HasComponent<RenderablesStorageComponent>();
}

void updateAnimation(const Ndk::EntityHandle& e)
{
    TealAssert(hasRightComponentsToAnimate(e), "Entity doesn't have the right components to animate");

    auto& sprites = e->GetComponent<RenderablesStorageComponent>().sprites;

    if (sprites.empty())
        return;

    auto& gfx = e->GetComponent<Ndk::GraphicsComponent>();
    auto& anim = e->GetComponent<AnimationComponent>();
    bool  moving = isEntityMoving(e);
    auto  dir = e->GetComponent<OrientationComponent>().dir;

    int const intDir = toint(dir);

    unsigned const startX = intDir * anim.size.x; // Get the x and the y
    unsigned const startY = anim.frame * anim.size.y;

    switch (anim.animationState)
    {
        case AnimationComponent::OnMove:
            for (auto sprite : sprites)
                onMoveAnimation(startX, startY, sprite, anim, moving);
            break;

        case AnimationComponent::OnEmote: // EmoteStore ?
            break;

        case AnimationComponent::OnFight:
            break;
    }
}

void onMoveAnimation(unsigned startX, unsigned startY, Nz::SpriteRef sprite, AnimationComponent& anim, bool moving)
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
