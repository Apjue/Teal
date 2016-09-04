// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPCOMPONENT_HPP
#define MAPCOMPONENT_HPP

#include <NDK/Component.hpp>
#include "micropather.h"
#include "util.hpp"

///
/// \class MapComponent
///
/// \brief Map of the game
///        Only one instance of it should exist
///

class MapComponent : public Ndk::Component<MapComponent>, public micropather::Graph
{
public:
    MapComponent() = default;
    inline MapComponent(const OLDTILEARRAY& _map, const OLDTILEARRAY& _obs);

    MapComponent(const MapComponent&) = default;
    MapComponent& operator=(const MapComponent&) = default;

    MapComponent(MapComponent&&) = default;
    MapComponent& operator=(MapComponent&&) = default;

    ~MapComponent() = default;

    OLDTILEARRAY map;
    OLDTILEARRAY obs;
    //0 = can pass, 1 = can't pass but can view through (in fight), 2 = can't pass and can't view through


    //Utility
    inline static void NodeToXY(void* node, unsigned& x, unsigned& y);
    inline static void* XYToNode(unsigned x, unsigned y);
    inline static void XYToArray(unsigned /*x*/, unsigned& y);

    static Ndk::ComponentIndex componentIndex;

private:
    bool passable(unsigned sX, unsigned sY, unsigned eX, unsigned eY);

    //Micropather
    virtual float LeastCostEstimate(void* nodeStart, void* nodeEnd) override;
    virtual void AdjacentCost(void* node, std::vector<micropather::StateCost>* neighbors) override;
    virtual void PrintStateInfo(void* /*node*/) override {}
};

#endif // MAPCOMPONENT_HPP

#include "mapcomponent.inl"
