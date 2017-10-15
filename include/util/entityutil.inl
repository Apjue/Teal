// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/common/animationcomponent.hpp"
#include "entityutil.hpp"

bool isMapEntity(const Ndk::EntityHandle& e)
{
    return e->HasComponent<Ndk::GraphicsComponent>() && e->HasComponent<Ndk::NodeComponent>()
        && e->HasComponent<DefaultGraphicsPosComponent>();
}

bool hasComponentsToChangeMap(const Ndk::EntityHandle& e)
{
    return e->HasComponent<PositionComponent>();
}

bool isItemEntity(const Ndk::EntityHandle& e)
{
    return e->HasComponent<Items::ItemComponent>();
}

bool isEntityMoving(const Ndk::EntityHandle& e)
{
    return e->HasComponent<PathComponent>() && (!e->GetComponent<PathComponent>().path.empty());
}

inline Nz::Vector2f getDefGfxPos(const Ndk::EntityHandle& e)
{
    if (e->HasComponent<AnimationComponent>())
    {
        auto& anim = e->GetComponent<AnimationComponent>();

        if (anim.currentAnimation != AnimationComponent::InvalidAnimationID)
            return anim.getCurrentAnimation().offset;
    }

    return e->GetComponent<DefaultGraphicsPosComponent>().xy;
}
