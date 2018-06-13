// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

void TilesetCore::add(unsigned key, const Nz::String& correspondance)
{
    m_cache.emplace(key, correspondance);
}

Nz::String TilesetCore::get(unsigned key) const
{
    auto it = m_cache.find(key);

    if (it != m_cache.end())
        return it->second;

    #ifdef TEAL_SAFE

    throw std::runtime_error { "Didn't found key in cache" };

    #endif // TEAL_SAFE

    return "not_found";
}

unsigned TilesetCore::get(const Nz::String& key) const
{
    for (auto it : m_cache)
        if (it.second == key)
            return it.first;

    #ifdef TEAL_SAFE

    throw std::runtime_error { "Didn't found key in cache" };

    #endif // TEAL_SAFE


    return 0u;
}
