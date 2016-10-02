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

    inline std::shared_ptr<MapData> add(Nz::Vector2i pos, const MapData& data);
    inline std::shared_ptr<MapData> get(Nz::Vector2i pos);

    inline void clear();

    ///
    /// \fn loadFromJSON
    ///
    /// \brief Clears the cache and loads maps from the JSON file
    ///
    /// \param file JSON file to load
    ///

    void loadFromJSON(const Nz::String& file);

    ///
    /// \fn addFromJSON
    ///
    /// \brief Loads maps from the JSON file
    ///        but does not clear the cache
    ///
    /// \param file JSON file to load
    /// \param overwrite Overwrite current maps if they're in the JSON file
    ///

    void addFromJSON(const Nz::String& file, bool overwrite = true);

    ///
    /// \fn writeToJSON
    ///
    /// \brief Write maps in cache to the JSON file
    ///
    /// \param file JSON file to write to
    ///

    void writeToJSON(const Nz::String& file);

private:
    Cache<Nz::Vector2i, MapData> m_cache;
};

#include "mapcore.inl"

#endif // MAPCORE_HPP
