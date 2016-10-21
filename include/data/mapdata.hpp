// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPDATA_HPP
#define MAPDATA_HPP

#include <Nazara/Graphics/Material.hpp>
#include "tiledata.hpp"
#include "def/gamedef.hpp"

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

private:
    TILEARRAY m_tiles;
    STRINGTILEARRAY m_map;
    UNSIGNEDTILEARRAY m_obs;

    void updateOldTileArray();
    void updateTileArray();
};

#include "mapdata.inl"

#endif // MAPDATA_HPP
