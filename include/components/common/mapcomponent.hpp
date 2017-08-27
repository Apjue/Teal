// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPCOMPONENT_HPP
#define MAPCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>
#include <Nazara/Graphics/TileMap.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Core/String.hpp>
#include <Nazara/Core/Error.hpp>
#include <memory>
#include "micropather.h"
#include "data/mapdata.hpp"
#include "cache/tilesetcore.hpp"

///
/// \class MapInstance
///
/// \brief Map of the game
///        Only one instance of it should exist
///

class MapInstance : public micropather::Graph
{
public:
    MapInstance(TilesetCore* tcore, TilesetCore* ftcore, const Ndk::EntityHandle& e);
    inline MapInstance(const MapDataRef& data, const Nz::String& tileset, const Nz::String& fightTileset,
                       TilesetCore* tcore, TilesetCore* ftcore, const Ndk::EntityHandle& e);

    MapInstance(const MapInstance&) = default;
    MapInstance& operator=(const MapInstance&) = default;

    MapInstance(MapInstance&&) = default;
    MapInstance& operator=(MapInstance&&) = default;

    ~MapInstance() override = default;

    void update();
    inline MapDataRef getMap() const;
    inline void setMap(MapDataRef newMap);

    inline void setFightMode(bool f);
    inline bool getFightMode() const;
    inline void toggleFightMode();

private:
    Ndk::EntityHandle m_entity;
    MapDataRef m_map; // You have to reset the pather after changing map

    Nz::MaterialRef m_tileset;
    Nz::MaterialRef m_fightTileset;
    Nz::TileMapRef m_tilemap;

    TilesetCore* m_tilesetCore {}; // Used to convert tile string to tile number
    TilesetCore* m_fightTilesetCore {};
    bool m_fightMode {};

    bool adjacentPassable(unsigned sX, unsigned sY, unsigned eX, unsigned eY);

    // Micropather
    virtual float LeastCostEstimate(void* nodeStart, void* nodeEnd) override;
    virtual void  AdjacentCost(void* node, std::vector<micropather::StateCost>* neighbors) override;
    virtual void  PrintStateInfo(void* /*node*/) override {}
};

///
/// \struct MapComponent
///
/// \brief Wrapper for MapInstance, so it doesn't move in memory
///        Because it is used by micropather via raw pointer
///

struct MapComponent : public Ndk::Component<MapComponent>
{
    MapComponent() = default;

    MapComponent(const MapComponent&) = default;
    MapComponent& operator=(const MapComponent&) = default;


    std::shared_ptr<MapInstance> map;

    template<class... Args>
    inline void init(Args&&... args);

    static Ndk::ComponentIndex componentIndex;

private:
    MapComponent(const MapComponent&&) = delete;
    MapComponent& operator=(const MapComponent&&) = delete;
};

#include "mapcomponent.inl"

#endif // MAPCOMPONENT_HPP
