// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPCOMPONENT_HPP
#define MAPCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <Nazara/Utility/Mesh.hpp>
#include <Nazara/Utility/StaticMesh.hpp>
#include <Nazara/Utility/VertexMapper.hpp>
#include <Nazara/Utility/IndexMapper.hpp>
#include <Nazara/Graphics/Model.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Core/String.hpp>
#include "micropather.h"
#include "data/mapdata.hpp"
#include "def/layerdef.hpp"
#include "util/assert.hpp"
#include "util/util.hpp"
#include "util/mapposutil.hpp"
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
    MapInstance(const Ndk::EntityHandle& e, TilesetCore* tcore);
    inline MapInstance(const MapDataRef& data, const Nz::String& tileset,
                       TilesetCore* tcore, const Ndk::EntityHandle& e);

    MapInstance(const MapInstance&) = default;
    MapInstance& operator=(const MapInstance&) = default;

    MapInstance(MapInstance&&) = default;
    MapInstance& operator=(MapInstance&&) = default;

    ~MapInstance() = default;

    Nz::MaterialRef m_mat; // Tileset texture
    Nz::ModelRef m_model; // Use SetMesh when mesh changed
    TilesetCore* m_tilesetCore; // Used to convert tile string to tile number

    bool update();
    inline MapDataRef getMap() const;
    inline void setMap(MapDataRef newMap);

private:
    Ndk::EntityHandle m_entity;
    MapDataRef m_map; // You have to reset the pather after changing map

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
