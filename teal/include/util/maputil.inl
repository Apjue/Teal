// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/util/logicentityidcomponent.hpp"
#include "maputil.hpp"

MapDataRef activateMapEntities(const MapDataRef& map)
{
    TealAssert(map, "map not valid !");

    for (auto& e : map->getEntities())
    {
        e->Enable(true);

        if (e->HasComponent<LogicEntityIdComponent>())
            if (e->GetComponent<LogicEntityIdComponent>().logicEntity.IsValid())
                e->GetComponent<LogicEntityIdComponent>().logicEntity->Enable(true);
    }

    return map;
}

MapDataRef deactivateMapEntities(const MapDataRef& map)
{
    TealAssert(map, "map not valid !");

    for (auto& e : map->getEntities())
    {
        e->Enable(false);

        if (e->HasComponent<LogicEntityIdComponent>())
            if (e->GetComponent<LogicEntityIdComponent>().logicEntity.IsValid())
                e->GetComponent<LogicEntityIdComponent>().logicEntity->Enable(false);
    }

    return map;
}


void uninitializeMapUtility()
{
    initializeMapUtility(nullptr, nullptr, {});
}
