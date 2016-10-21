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

    MapData(const OLDTILEARRAY& map_, const OLDTILEARRAY& obs_);
    MapData(const TILEARRAY& tiles_);
    
    inline const TILEARRAY& tiles() const;
    inline const OLDTILEARRAY& map() const;
    inline const OLDTILEARRAY& obs() const;

    inline void setTiles(const TILEARRAY& nTiles);
    inline void setMap(const OLDTILEARRAY& nMap);
    inline void setObs(const OLDTILEARRAY& nObs);

private:
    TILEARRAY m_tiles;
    OLDTILEARRAY m_map;
    OLDTILEARRAY m_obs;

    void updateOldTileArray();
    void updateTileArray();
};

#include "mapdata.inl"

#endif // MAPDATA_HPP
