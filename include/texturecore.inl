// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEXTURECORE_INL
#define TEXTURECORE_INL

inline void TextureCore::setPrefix(const Nz::String& newPrefix)
{
    m_prefix = newPrefix;
}

inline const Nz::String& TextureCore::getPrefix() const noexcept
{
    return m_prefix;
}

inline Nz::TextureRef TextureCore::get(const Nz::String& key) const
{
    return m_cache.get(key);
}

#endif
