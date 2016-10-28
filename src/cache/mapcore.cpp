// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "cache/mapcore.hpp"

void MapCore::loadFromJSON(const Nz::String& file)
{
    clear();
    addFromJSON(file);
}

void MapCore::addFromJSON(const Nz::String& file, bool overwrite)
{
    NazaraAssert(false, "Function not made yet");
}

void MapCore::writeToJSON(const Nz::String& file)
{
    NazaraAssert(false, "Function not made yet");
}
