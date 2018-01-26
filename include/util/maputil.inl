// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

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

Nz::String mapXYToString(int x, int y)
{
    Nz::String str;

    str.Append(Nz::String::Number(x));
    str.Append(';');
    str.Append(Nz::String::Number(y));

    return str;
}

std::pair<int, int> stringToMapXY(const Nz::String& str)
{
    long long lx {}, ly {};
    std::vector<Nz::String> result;
    unsigned splits = str.Split(result, ';');

    TealAssert(splits == 2, "Not a map position");

    result[0].ToInteger(&lx);
    result[1].ToInteger(&ly);

    int x = int(lx);
    int y = int(ly);

    return std::make_pair(x, y);
}

void uninitializeMapUtility()
{
    initializeMapUtility(nullptr, nullptr, {});
}

Nz::Rectui getTileAABB(unsigned x, unsigned y)
{
    return Nz::Rectui { x * Def::TileSizeX + isLineEven(y) ? 0u : Def::TileSizeX / 2, y / 2 * Def::TileSizeY, Def::TileSizeX, Def::TileSizeY };
}

Nz::Vector2ui getTileCenter(unsigned x, unsigned y)
{
    Nz::Rectui aabb = getTileAABB(x, y);
    return { aabb.x + aabb.width / 2, aabb.y + aabb.height / 2 };
}
