// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

void activateMapEntities(const std::shared_ptr<MapData>& map)
{
    NazaraAssert(map, "map not valid !");

    for (auto& e : map->getEntities())
        e->Enable();
}

void deactivateMapEntities(const std::shared_ptr<MapData>& map)
{
    NazaraAssert(map, "map not valid !");

    for (auto& e : map->getEntities())
        e->Enable(false);
}

bool hasComponentsToChangeMap(const Ndk::EntityHandle& p)
{
    return p->HasComponent<PositionComponent>()
        && p->HasComponent<MapPositionComponent>();
}
