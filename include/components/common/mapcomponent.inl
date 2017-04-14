// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

MapInstance::MapInstance(const MapDataRef& data, const Nz::String& tileset,
                         TilesetCore* tcore, const Ndk::EntityHandle& e)
    : MapInstance(e, tcore)
{
    m_map = data;

    if (!m_mat->SetDiffuseMap(tileset))
        NazaraError("Error: Map Material SetDiffuseMap failed !");

    update();
}

MapDataRef MapInstance::getMap() const
{
    return m_map;
}

void MapInstance::setMap(MapDataRef newMap)
{
    m_map = newMap;
    m_map->updateOccupiedTiles();
}


template<class... Args>
void MapComponent::init(Args&&... args)
{
    map = std::make_shared<MapInstance>(std::forward<Args>(args)..., m_entity);
}
