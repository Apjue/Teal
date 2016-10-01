// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPCORE_HPP
#define MAPCORE_HPP

#include <Nazara/Math/Vector2.hpp>
#include "cache.hpp"
#include "data/mapdata.hpp"

///
/// \class MapCore
///
/// \brief Stores and create MapData
///
/// \todo Load from JSON, add from JSON, write to JSON
///

class MapCore
{
public:
    MapCore() = default;
    ~MapCore() = default;

    std::shared_ptr<MapData> add(Nz::Vector2i pos, const MapData& data);
    std::shared_ptr<MapData> get(Nz::Vector2i pos);

private:
    Cache<Nz::Vector2i, MapData> m_cache;
};

#endif // MAPCORE_HPP
