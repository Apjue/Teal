// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TILESETCORE_HPP
#define TILESETCORE_HPP

#include <Nazara/Core/String.hpp>
#include <unordered_map>

///
/// \class TilesetCore
///
/// \brief This class wraps a cache used to convert
///        unsigned number to String for tilesets
///

class TilesetCore
{
public:
    using InternalCache = std::unordered_map<unsigned, Nz::String>;

    TilesetCore() = default;
    ~TilesetCore() = default;

    inline void add(unsigned key, const Nz::String& correspondance);

    inline Nz::String get(unsigned key) const;
    inline unsigned get(const Nz::String& key) const;

private:
    InternalCache m_cache;
};

#include "tilesetcore.inl"

#endif // TILESETCORE_HPP
