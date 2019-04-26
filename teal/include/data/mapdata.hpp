// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPDATA_HPP
#define MAPDATA_HPP

#include <NDK/Entity.hpp>
#include <NDK/EntityList.hpp>
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
#include "global.hpp"

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
    MapData(const TileArray& tiles_);

    MapData(MapData&&) = default;
    MapData& operator=(MapData&&) = default;

    template<class... Args>
    static inline MapDataRef New(Args&&... args);

    inline const Nz::Vector2i& getPosition() const;
    inline void setPosition(const Nz::Vector2i& pos);

    inline const TileArray& getTiles() const;
    inline void setTiles(const TileArray& nTiles);

    inline const TileData& getTile(unsigned x, unsigned y) const;
    inline const TileData& getTile(unsigned index) const;

    std::unordered_map<Nz::Vector2ui, TileData> adjacentTiles(unsigned x, unsigned y);
    inline std::unordered_map<Nz::Vector2ui, TileData> adjacentTiles(unsigned index);

    inline const Ndk::EntityList& getEntities() const;
    inline Ndk::EntityList& getEntities();

    void updateOccupiedTiles();

private:
    Nz::Vector2i m_pos {};
    TileArray m_tiles;
    Ndk::EntityList m_entities; // Usable Objects, NPCs, decorations, etc.

    static MapDataLibrary::LibraryMap s_library;
};

#include <Nazara/Lua/LuaState.hpp>
#include <NDK/LuaAPI.hpp>

namespace Nz
{

extern unsigned int LuaImplQueryArg(const LuaState& state, int index, MapDataRef* mapRef, TypeTag<MapDataRef>);
//extern int LuaImplReplyVal(const LuaState& state, MapDataRef&& mapRef, TypeTag<MapDataRef>);

} // namespace Nz

#include "mapdata.inl"

#endif // MAPDATA_HPP
