// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

std::shared_ptr<MapData> MapCore::add(Nz::Vector2i pos, const MapData& data)
{
    return m_cache.add(pos, data);
}

std::shared_ptr<MapData> MapCore::get(Nz::Vector2i pos)
{
    return m_cache.get(pos);
}

void MapCore::clear()
{
    m_cache.clear();
}
