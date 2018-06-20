// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
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

bool hasComponentsToAnimate(const Ndk::EntityHandle& e)
{
    return e->HasComponent<AnimationComponent>() &&
        e->HasComponent<Ndk::GraphicsComponent>() &&
        e->HasComponent<PositionComponent>() &&
        e->HasComponent<OrientationComponent>() &&
        e->HasComponent<RenderablesStorageComponent>();
}

void updateAnimation(const Ndk::EntityHandle& e)
{
    TealAssert(hasComponentsToAnimate(e), "Entity doesn't have the right components to animate");

    auto& sprites = e->GetComponent<RenderablesStorageComponent>().sprites;

    if (sprites.empty())
        return;

    auto& anim = e->GetComponent<AnimationComponent>();

    if (anim.currentAnimation == AnimationComponent::InvalidAnimationID)
        return;

    TealAssert(!anim.animList.empty(), "Animation list must not be empty, as currentAnimation isn't set to InvalidAnimationID");

    AnimationData& animData = anim.getCurrentAnimation();
    auto orientation = e->GetComponent<OrientationComponent>().orientation;

    unsigned const startX = unsigned(orientation) * animData.size.x; // Get the x and the y
    unsigned const startY = animData.frame * animData.size.y;

    for (auto& sprite : sprites)
        animate(startX, startY, sprite, animData, isEntityMoving(e));
}

void animate(unsigned startX, unsigned startY, const Nz::SpriteRef& sprite, AnimationData& animData, bool moving)
{
    sprite->SetTexture(animData.texture, false);
    unsigned maxframe = (sprite->GetMaterial()->GetDiffuseMap()->GetSize().y / animData.size.y) - 1u; // Sprites always use the y axis for animations

    switch (animData.type)
    {
        case AnimationData::Walk:
            sprite->SetTextureRect({ startX, (!moving || maxframe == 0 ? 0 : startY), animData.size.x, animData.size.y });
            animData.frame = (!moving || maxframe == 0 || animData.frame >= maxframe ? 0 : animData.frame + 1);
            break;

        default:
            sprite->SetTextureRect({ startX, startY, animData.size.x, animData.size.y });
            animData.frame = (animData.frame >= maxframe ? 0 : animData.frame + 1);
            break;
    }
}
