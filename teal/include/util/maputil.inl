// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/util/logicentityidcomponent.hpp"
#include "maputil.hpp"

MapDataRef activateMapEntities(const MapDataRef& map)
{
    TealAssert(map, "map not valid !");

    for (auto& e : map->getGraphicalEntities())
    {
        e->Enable();

        if (e->HasComponent<LogicEntityIdComponent>())
            if (e->GetComponent<LogicEntityIdComponent>().logicEntity.IsValid())
                e->GetComponent<LogicEntityIdComponent>().logicEntity->Enable(true);
    }

    for (auto& e : map->getMonsterGroups())
        e->Enable();

    return map;
}

MapDataRef deactivateMapEntities(const MapDataRef& map)
{
    TealAssert(map, "map not valid !");

    for (auto& e : map->getGraphicalEntities())
    {
        e->Enable(false);

        if (e->HasComponent<LogicEntityIdComponent>())
            if (e->GetComponent<LogicEntityIdComponent>().logicEntity.IsValid())
                e->GetComponent<LogicEntityIdComponent>().logicEntity->Enable(false);
    }

    for (auto& e : map->getMonsterGroups())
        e->Enable(false);

    return map;
}


void uninitializeMapUtility()
{
    initializeMapUtility(nullptr, nullptr, {});
}
