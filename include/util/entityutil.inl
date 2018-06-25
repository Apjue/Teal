// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include "components/common/defaultgraphicsposcomponent.hpp"
#include "components/common/pathcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/logicentityidcomponent.hpp"
#include "components/common/monstercomponent.hpp"
#include "components/common/fightcomponent.hpp"
#include "components/common/lifecomponent.hpp"
#include "components/items/itemcomponent.hpp"
#include "entityutil.hpp"

bool isMapEntity(const Ndk::EntityHandle& e)
{
    return e->HasComponent<Ndk::GraphicsComponent>() && e->HasComponent<Ndk::NodeComponent>() && e->HasComponent<DefaultGraphicsPosComponent>();
}

bool isItemEntity(const Ndk::EntityHandle& e)
{
    return e->HasComponent<ItemComponent>();
}

bool isMonsterEntity(const Ndk::EntityHandle& e)
{
    return e->HasComponent<MonsterComponent>();
}

bool isFightableEntity(const Ndk::EntityHandle& e)
{
    return e->HasComponent<FightComponent>() && e->HasComponent<LifeComponent>();
}

bool isGraphicalItemEntity(const Ndk::EntityHandle& e)
{
    return isMapEntity(e) && e->HasComponent<LogicEntityIdComponent>();
}

bool isValidGraphicalItemEntity(const Ndk::EntityHandle& e)
{
    return isGraphicalItemEntity(e) && e->GetComponent<LogicEntityIdComponent>().logicEntity.IsValid();
}

bool isEntityMoving(const Ndk::EntityHandle& e)
{
    return e->HasComponent<PathComponent>() && !(e->GetComponent<PathComponent>().path.empty());
}

bool isEntityRunning(const Ndk::EntityHandle& e)
{
    return e->HasComponent<PathComponent>() && (e->GetComponent<PathComponent>().path.size() > 1);
}

bool hasComponentsToChangeMap(const Ndk::EntityHandle& e)
{
    return e->HasComponent<PositionComponent>();
}

Nz::Vector2f getDefGfxPos(const Ndk::EntityHandle& e)
{
    if (e->HasComponent<AnimationComponent>())
    {
        auto& anim = e->GetComponent<AnimationComponent>();
        auto  animType = determineAnimationToBeUsed(e);

        if (anim.canAnimationBeUsed(animType))
            return anim.animList[animType].offset;
    }

    return e->GetComponent<DefaultGraphicsPosComponent>().offset;
}

AnimationComponent::AnimationType determineAnimationToBeUsed(const Ndk::EntityHandle& e)
{
    if (isEntityRunning(e))
        return AnimationComponent::Run;

    if (isEntityMoving(e))
        return AnimationComponent::Walk;

    // Other cases
    return AnimationComponent::Walk;
}
