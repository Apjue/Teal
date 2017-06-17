// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATEMETADATA_HPP
#define STATEMETADATA_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Renderer/Texture.hpp>

struct StateMetaData
{
    Nz::String name;
    Nz::String description;
    Nz::TextureRef icon;
};

#endif // STATEMETADATA_HPP
