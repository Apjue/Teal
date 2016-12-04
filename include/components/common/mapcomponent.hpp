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
#include <cstdint>
#include "micropather.h"
#include "data/mapdata.hpp"
#include "def/layerdef.hpp"
#include "util/util.hpp"
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
    MapInstance(const MapDataRef& data, const Nz::String& tileset,
                TilesetCore* tcore, const Ndk::EntityHandle& e);

    MapInstance(const MapInstance&) = default;
    MapInstance& operator=(const MapInstance&) = default;

    MapInstance(MapInstance&&) = default;
    MapInstance& operator=(MapInstance&&) = default;

    ~MapInstance() = default;

    MapDataRef map; // You have to reset the pather after changing map

    Nz::MaterialRef m_mat; // Tileset texture
    Nz::ModelRef m_model; // Use SetMesh when mesh changed
    TilesetCore* m_tilesetCore; // Used to convert tile string to tile number

    bool update();

    // Utility
    static void  NodeToXY(void* node, unsigned& x, unsigned& y);
    static void* XYToNode(unsigned x, unsigned y);
    static void  XYToArray(unsigned /*x*/, unsigned& y);
    static std::pair<unsigned, unsigned> IndexToXY(unsigned index);
    static unsigned XYToIndex(unsigned x, unsigned y);

private:
    Ndk::EntityHandle m_entity;

    bool passable(unsigned sX, unsigned sY, unsigned eX, unsigned eY);

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
    void init(Args&&... args)
    {
        map = std::make_shared<MapInstance>(std::forward<Args>(args)..., m_entity);
    }

    static Ndk::ComponentIndex componentIndex;

private:
    MapComponent(const MapComponent&&) = delete;
    MapComponent& operator=(const MapComponent&&) = delete;
};

#endif // MAPCOMPONENT_HPP
