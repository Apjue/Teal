// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

bool isMapEntity(const Ndk::EntityHandle& e)
{
    return e->HasComponent<Ndk::GraphicsComponent>() && e->HasComponent<Ndk::NodeComponent>()
        && e->HasComponent<DefaultGraphicsPosComponent>();
}

bool hasComponentsToChangeMap(const Ndk::EntityHandle& e)
{
    return e->HasComponent<PositionComponent>()
        && e->HasComponent<MapPositionComponent>();
}

bool isItemEntity(const Ndk::EntityHandle& e)
{
    return e->HasComponent<Items::ItemComponent>();
}
