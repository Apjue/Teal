// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components/GraphicsComponent.hpp>
#include "components/characters/orientationcomponent.hpp"
#include "components/characters/pathcomponent.hpp"
#include "components/characters/positioncomponent.hpp"
#include "components/util/renderablesstoragecomponent.hpp"
#include "def/gamedef.hpp"
#include "def/systemdef.hpp"
#include "util/gfxutil.hpp"
#include "util/assert.hpp"
#include "util/entityutil.hpp"
#include "util/animutil.hpp"

bool hasComponentsToAnimate(const Ndk::EntityHandle& e)
{
    return e->HasComponent<AnimationComponent>() &&
           e->HasComponent<OrientationComponent>() &&
           e->HasComponent<RenderablesStorageComponent>();
}

void updateAnimation(const Ndk::EntityHandle& e)
{
    TealAssert(hasComponentsToAnimate(e), "Entity doesn't have the required components to animate");

    auto& sprites = e->GetComponent<RenderablesStorageComponent>().sprites;

    if (sprites.empty())
        return;

    auto& anim = e->GetComponent<AnimationComponent>();

    if (anim.animList.empty())
        return;

    auto animType = determineAnimationToBeUsed(e);

    if (!anim.canAnimationBeUsed(animType))
    {
        if (!anim.canAnimationBeUsed(AnimationComponent::Walk))
            return;

        else
            animType = AnimationComponent::Walk;
    }

    AnimationData& animData = anim.animList[animType];

    auto orientation = e->GetComponent<OrientationComponent>().orientation;

    unsigned const startX = unsigned(orientation) * animData.size.x; // Get the x and the y
    unsigned const startY = animData.frame * animData.size.y;

    for (auto& sprite : sprites)
        animate({ startX, startY }, sprite, animData, animType, e->HasComponent<PathComponent>() ? e->GetComponent<PathComponent>().path.size() : 0);
}

void animate(Nz::Vector2ui startCoords, const Nz::SpriteRef& sprite, AnimationData& animData, AnimationComponent::AnimationType animType, std::size_t pathSize)
{
    sprite->SetTexture(animData.texture, false);
    unsigned maxframe = (sprite->GetMaterial()->GetDiffuseMap()->GetSize().y / animData.size.y) - 1u; // Sprites always use the y axis for animations

    bool moving = (pathSize != 0);

    switch (animType)
    {
        case AnimationComponent::Walk:
            sprite->SetTextureRect({ startCoords.x, (!moving || maxframe == 0 ? 0 : startCoords.y), animData.size.x, animData.size.y });
            animData.frame = (!moving || maxframe == 0 || animData.frame >= maxframe ? 0 : animData.frame + 1);
            break;

        case AnimationComponent::Run:
            sprite->SetTextureRect({ startCoords.x, (!moving || maxframe == 0 ? 0 : startCoords.y), animData.size.x, animData.size.y });
            animData.frame = (!moving || maxframe == 0 || animData.frame >= maxframe ? 0 : animData.frame + 1);
            break;

        default:
            sprite->SetTextureRect({ startCoords.x, startCoords.y, animData.size.x, animData.size.y });
            animData.frame = (animData.frame >= maxframe ? 0 : animData.frame + 1);
            break;
    }
}
