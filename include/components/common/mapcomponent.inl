// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

MapInstance::MapInstance(const MapDataRef& data, const Nz::String& tileset, const Nz::String& fightTileset,
                         TilesetCore* tcore, TilesetCore* ftcore, const Ndk::EntityHandle& e)
    : MapInstance(tcore, ftcore, e)
{
    m_map = data;

    if (!m_tileset->SetDiffuseMap(tileset))
        NazaraError("Error: Map Material SetDiffuseMap failed !");

    if (!m_fightTileset->SetDiffuseMap(fightTileset))
        NazaraError("Error: Map Fight Material SetDiffuseMap failed !");

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


inline void MapInstance::setFightMode(bool f)
{
    m_fightMode = f;
}

inline bool MapInstance::getFightMode() const
{
    return m_fightMode;
}

inline void MapInstance::toggleFightMode()
{
    setFightMode(!m_fightMode);
}


template<class... Args>
void MapComponent::init(Args&&... args)
{
    map = std::make_shared<MapInstance>(std::forward<Args>(args)..., m_entity);
}
