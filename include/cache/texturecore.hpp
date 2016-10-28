// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEXTURECORE_HPP
#define TEXTURECORE_HPP

#include "cache.hpp"
#include <Nazara/Core/String.hpp>
#include <Nazara/Renderer/Texture.hpp>

class TextureCore
{
public:
    TextureCore() = default;
    ~TextureCore() = default;

    inline void setPrefix(const Nz::String& newPrefix);
    inline const Nz::String& getPrefix() const noexcept;

    inline Nz::TextureRef get(const Nz::String& key) const;
    bool addByLoad(const Nz::String& key, const Nz::String& filepath);
    void add(const Nz::String& key, const Nz::TextureRef& ref);

private:
    Cache<Nz::String, Nz::Texture, NzObjectRefCopier<Nz::Texture>> m_cache;
    Nz::String m_prefix {};
};

#include "texturecore.inl"

#endif // TEXTURECORE_HPP