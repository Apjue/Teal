// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

inline MapInstance::MapInstance(const MapDataRef& data, const Nz::String& tileset, const Nz::String& fightTileset,
                         TilesetCore* tcore, TilesetCore* ftcore, const Ndk::EntityHandle& e)
    : MapInstance(tcore, ftcore, e)
{
    m_maps[0] = data;


    if (!m_tileset->SetDiffuseMap(tileset))
        NazaraError("Error: Map Material SetDiffuseMap failed !");

    if (!m_fightTileset->SetDiffuseMap(fightTileset))
        NazaraError("Error: Map Fight Material SetDiffuseMap failed !");

    update();
}

inline MapDataRef MapInstance::getMap(int index)
{
    return m_maps[index];
}

inline MapDataRef MapInstance::getCurrentMap() const
{
    return m_maps.at(getCurrentMapIndex());
}

inline void MapInstance::setMap(int index, MapDataRef newMap)
{
    m_maps[index] = newMap;
    m_maps[index]->updateOccupiedTiles();
}

inline int MapInstance::getCurrentMapIndex() const
{
    return m_mapIndex;
}

inline void MapInstance::switchMap(int index)
{
    m_mapIndex = index;
    update();
}


inline void MapInstance::setFightMode(bool f)
{
    m_fightMode = f;
    update();
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
