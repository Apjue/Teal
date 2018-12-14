// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPCOMPONENT_HPP
#define MAPCOMPONENT_HPP

#include <NDK/World.hpp>
#include <NDK/Component.hpp>
#include <Nazara/Graphics/TileMap.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Core/String.hpp>
#include <Nazara/Core/Error.hpp>
#include <memory>
#include <array>
#include <unordered_map>
#include "micropather.h"
#include "cache/tilesetcore.hpp"
#include "data/mapdata.hpp"
#include "def/gamedef.hpp"

///
/// \class MapInstance
///
/// \brief Map of the game
///        Only one instance of it should exist
///

class MapInstance : public micropather::Graph
{
    static const unsigned s_borderTilesNumberX { Def::MapX };
    static const unsigned s_downBorderTilesNumber { s_borderTilesNumberX % 2 == 0 ? s_borderTilesNumberX : s_borderTilesNumberX + 1 };
    static const unsigned s_borderTilesNumberY { Def::MapY / 2 + 2u };

public:
    MapInstance(const TilesetCore* tcore, const TilesetCore* ftcore, const Ndk::EntityHandle& e);
    inline MapInstance(const MapDataRef& data, const Nz::String& tileset, const Nz::String& fightTileset,
                       const TilesetCore* tcore, const TilesetCore* ftcore, const Ndk::EntityHandle& e);

    MapInstance(const MapInstance&) = default;
    MapInstance& operator=(const MapInstance&) = default;

    MapInstance(MapInstance&&) = default;
    MapInstance& operator=(MapInstance&&) = default;

    ~MapInstance() override = default;

    void update();
    void updateBorders();

    inline MapDataRef getMap(int index);
    inline MapDataRef getCurrentMap() const;
    inline void setMap(int index, MapDataRef newMap);

    inline int getCurrentMapIndex() const;
    inline void switchMap(int index);

    inline void setFightMode(bool f);
    inline bool getFightMode() const;
    inline void toggleFightMode();

private:
    Ndk::WorldHandle m_world;
    Ndk::EntityHandle m_entity;

    std::unordered_map<int, MapDataRef> m_maps; // You have to clear the pather's cache after changing map
    int m_mapIndex {};

    Nz::MaterialRef m_tileset;
    Nz::MaterialRef m_fightTileset;
    Nz::TileMapRef m_tilemap;

    const TilesetCore* m_tilesetCore {}; // Used to convert tile string to tile number
    const TilesetCore* m_fightTilesetCore {};
    bool m_fightMode {};

    Ndk::EntityHandle m_borderEntity;
    std::array<Nz::SpriteRef, s_borderTilesNumberY> m_leftBorder;
    std::array<Nz::SpriteRef, s_borderTilesNumberY> m_rightBorder;
    std::array<Nz::SpriteRef, s_borderTilesNumberX> m_upBorder;
    std::array<Nz::SpriteRef, s_downBorderTilesNumber> m_downBorder;

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
