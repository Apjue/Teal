// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPDATA_HPP
#define MAPDATA_HPP

#include <NDK/Entity.hpp>
#include <NDK/EntityList.hpp>
#include <Nazara/Graphics/Material.hpp>
#include "tiledata.hpp"
#include "def/typedef.hpp"

struct MapData
{
public:
    MapData() = default;
    ~MapData() = default;

    MapData(const STRINGTILEARRAY& map_, const UNSIGNEDTILEARRAY& obs_);
    MapData(const TILEARRAY& tiles_);
    
    inline const TILEARRAY& tiles() const;
    inline const STRINGTILEARRAY& map() const;
    inline const UNSIGNEDTILEARRAY& obs() const;

    inline void setTiles(const TILEARRAY& nTiles);
    inline void setMap(const STRINGTILEARRAY& nMap);
    inline void setObs(const UNSIGNEDTILEARRAY& nObs);

    inline void addEntity(const Ndk::EntityHandle& e);
    inline const Ndk::EntityList& getEntities() const;
    inline Ndk::EntityList& getEntities();

private:
    TILEARRAY m_tiles;
    STRINGTILEARRAY m_map;
    UNSIGNEDTILEARRAY m_obs;
    Ndk::EntityList m_entities; // Usable Objects, NPCs, decoration, etc.

    void updateOldTileArray();
    void updateTileArray();
};

#include "mapdata.inl"

#endif // MAPDATA_HPP
