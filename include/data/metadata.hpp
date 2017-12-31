// Copyright (C) 2018 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATEMETADATA_HPP
#define STATEMETADATA_HPP

#include <Nazara/Core/String.hpp>

struct StateMetaData
{
    Nz::String name;
    Nz::String description;
    Nz::String icon;
};

struct EffectMetaData
{
    Nz::String name;
    Nz::String description;
    Nz::String icon;
};

#endif // STATEMETADATA_HPP
