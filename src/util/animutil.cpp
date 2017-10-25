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
#include "components/common/namecomponent.hpp"
void updateAnimation(const Ndk::EntityHandle& e)
{
    TealAssert(hasRightComponentsToAnimate(e), "Entity doesn't have the right components to animate");

    auto& sprites = e->GetComponent<RenderablesStorageComponent>().sprites;

    if (sprites.empty())
        return;

    auto& anim = e->GetComponent<AnimationComponent>();

    if (anim.currentAnimation == AnimationComponent::InvalidAnimationID || anim.animList.empty())
        return;

    Nz::String name;
    if (e->HasComponent<NameComponent>())
        name = e->GetComponent<NameComponent>().name;

    AnimationData animData = anim.getCurrentAnimation();
    bool  moving = isEntityMoving(e);
    auto  dir = e->GetComponent<OrientationComponent>().dir;

    int const intDir = toint(dir);

    unsigned const startX = intDir * animData.size.x; // Get the x and the y
    unsigned const startY = animData.frame * animData.size.y;

    for (auto sprite : sprites)
        animate(startX, startY, sprite, animData, moving);
}

void animate(unsigned startX, unsigned startY, Nz::SpriteRef sprite, AnimationData& animData, bool moving)
{
    sprite->SetTexture(animData.texture, false);
    unsigned maxframe = sprite->GetMaterial()->GetDiffuseMap()->GetSize().y / animData.size.y; // Sprites always use the y axis for animations

    switch (animData.type)
    {
        case AnimationData::Walk:
            if (!moving || maxframe == 0) // Reset animation, and change direction
            {
                animData.frame = 0;
                sprite->SetTextureRect({ startX, 0u, animData.size.x, animData.size.y });

                return;
            }

            else // Animation !
            {
                sprite->SetTextureRect({ startX, startY, animData.size.x, animData.size.y });
                ++animData.frame;

                if (animData.frame > maxframe)
                    animData.frame = 0;
            }

            break;

        default:
            sprite->SetTextureRect({ startX, startY, animData.size.x, animData.size.y });
            ++animData.frame;

            if (animData.frame > maxframe)
                animData.frame = 0;
            break;
    }
}
