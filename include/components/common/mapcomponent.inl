// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPCOMPONENT_INL
#define MAPCOMPONENT_INL

MapComponent::MapComponent(const OLDTILEARRAY& _map,
                           const OLDTILEARRAY& _obs)
    : map(_map), obs(_obs) {}

void MapComponent::NodeToXY(void* node, unsigned& x, unsigned& y)
{
    int index = (int) node;
    auto xy = IndexToXY(static_cast<unsigned>(index));

    x = xy.first;
    y = xy.second;
}

void* MapComponent::XYToNode(unsigned x, unsigned y)
{
    return (void*) (y * Def::MAPX + x);
}

void MapComponent::XYToArray(unsigned /*x*/, unsigned& y)
{
    y /= 2;
}

std::pair<unsigned, unsigned> MapComponent::IndexToXY(unsigned index)
{
    unsigned x {}, y {};

    y = index / Def::MAPX;
    x = index - y * Def::MAPX;

    return std::make_pair(x, y);
}

#endif // MAPCOMPONENT_INL