// Copyright (C) 2016 Samy Bensaid
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

    TealAssert(str.Split(result, ';') == 2, "Not a map position");

    result[0].ToInteger(&lx);
    result[1].ToInteger(&ly);

    int x = lltoi(ly);
    int y = lltoi(ly);

    return std::make_pair(x, y);
}
