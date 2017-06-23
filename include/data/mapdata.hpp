// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPDATA_HPP
#define MAPDATA_HPP

#include <NDK/Entity.hpp>
#include <NDK/EntityList.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Core/ObjectLibrary.hpp>
#include <Nazara/Core/ObjectRef.hpp>
#include <Nazara/Core/RefCounted.hpp>
#include <Nazara/Core/Resource.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <unordered_map>
#include "tiledata.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/blocktilecomponent.hpp"
#include "def/typedef.hpp"
#include "def/gamedef.hpp"
#include "util/mapposutil.hpp"

class MapData;

using MapDataConstRef = Nz::ObjectRef<const MapData>;
using MapDataLibrary = Nz::ObjectLibrary<MapData>;
using MapDataRef = Nz::ObjectRef<MapData>;

class MapData : public Nz::RefCounted, public Nz::Resource
{
    friend MapDataLibrary;

public:
    MapData() = default;
    ~MapData() = default;

    MapData(const STRINGTILEARRAY& map_, const STRINGTILEARRAY& obs_);
    MapData(const TILEARRAY& tiles_);

    MapData(MapData&&) = default;

    template<class... Args>
    static inline MapDataRef New(Args&&... args);

    inline const TILEARRAY& tiles() const;
    inline const STRINGTILEARRAY& map() const; // Todo: Delete. Only Use Tile Array
    inline const STRINGTILEARRAY& obs() const; // Todo: ^

    inline const TileData& tile(unsigned x, unsigned y) const;
    inline const TileData& tile(unsigned index) const;
    inline std::unordered_map<Nz::Vector2ui, TileData> adjacentTiles(unsigned x, unsigned y);

    inline void setTiles(const TILEARRAY& nTiles);
    inline void setMap(const STRINGTILEARRAY& nMap); // Todo: Delete. Only Use Tile Array
    inline void setObs(const STRINGTILEARRAY& nObs); // Todo: ^

    inline const Ndk::EntityList& getEntities() const;
    inline Ndk::EntityList& getEntities();

    void updateOccupiedTiles();

private:
    TILEARRAY m_tiles;
    STRINGTILEARRAY m_map;
    STRINGTILEARRAY m_obs;
    Ndk::EntityList m_entities; // Usable Objects, NPCs, decorations, etc.

    static MapDataLibrary::LibraryMap s_library;

    void updateOldTileArray();
    void updateTileArray();

    inline unsigned XYToIndex(unsigned x, unsigned y) const;
};

#include "mapdata.inl"

#endif // MAPDATA_HPP
