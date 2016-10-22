// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

void activateMapEntities(MapData& map)
{
    for (auto& e : map.getEntities())
        e->Enable();
}

void deactivateMapEntities(MapData& map)
{
    for (auto& e : map.getEntities())
        e->Enable(false);
}

bool hasComponentsToChangeMap(const Ndk::EntityHandle& p)
{
    return p->HasComponent<PositionComponent>()
        && p->HasComponent<MapPositionComponent>();
}
