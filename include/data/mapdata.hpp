// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPDATA_HPP
#define MAPDATA_HPP

#include <NDK/Entity.hpp>
#include <Nazara/Core/ObjectLibrary.hpp>
#include <Nazara/Core/ObjectRef.hpp>
#include <Nazara/Core/RefCounted.hpp>
#include <Nazara/Core/Resource.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <memory>
#include <vector>
#include "def/typedef.hpp"
#include "data/tiledata.hpp"
#include "util/mapposutil.hpp"

class MapData;

using MapDataConstRef = Nz::ObjectRef<const MapData>;
using MapDataLibrary = Nz::ObjectLibrary<MapData>;
using MapDataRef = Nz::ObjectRef<MapData>;

class MapData : public Nz::RefCounted, public Nz::Resource /// \todo Remove MapPositionComponent and add position in mapdata ?
{
    friend MapDataLibrary;

public:
    using EntityList = std::vector<Ndk::EntityHandle>;

    MapData() = default;
    ~MapData() = default;

    MapData(const TileArray& tiles_);
    MapData(MapData&&) = default;

    template<class... Args>
    static inline MapDataRef New(Args&&... args);

    inline const Nz::Vector2i& getPosition() const;
    inline void setPosition(const Nz::Vector2i& pos);

    inline const TileArray& tiles() const;
    inline void setTiles(const TileArray& nTiles);

    inline const TileData& tile(unsigned x, unsigned y) const;
    inline const TileData& tile(unsigned index) const;
    std::unordered_map<Nz::Vector2ui, TileData> adjacentTiles(unsigned x, unsigned y);

    inline const EntityList& getEntities() const;
    inline EntityList& getEntities();

    void updateOccupiedTiles();

private:
    Nz::Vector2i m_pos {};
    TileArray m_tiles;
    EntityList m_entities; // Usable Objects, NPCs, decorations, etc.

    static MapDataLibrary::LibraryMap s_library;
};

#include "mapdata.inl"

#endif // MAPDATA_HPP
