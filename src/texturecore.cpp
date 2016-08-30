// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "texturecore.hpp"

bool TextureCore::addByLoad(const Nz::String& key, const Nz::String& filepath)
{
    Nz::String path = m_prefix + filepath;

    Nz::TextureRef toAdd = Nz::Texture::New();
    if (!toAdd->LoadFromFile(path))
        return false;

    add(key, toAdd);
    return true;
}

void TextureCore::add(const Nz::String& key, const Nz::TextureRef& ref)
{
    m_cache.add(key, ref);
}
