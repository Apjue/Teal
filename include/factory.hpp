// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <NDK/World.hpp>
#include <NDK/Entity.hpp>
#include <NDK/Components.hpp>
#include "components.hpp"
#include "util/gfxutil.hpp"
#include "def/layerdef.hpp"
#include "data/characterdata.hpp"
//#include "data/mapobjectdata.hpp"

///
/// \fn make_character
///
/// \brief Creates a character with all required components
///
/// \note There's no difference between a player and a NPC
///       But only NPCs have the RandomMovement component
///

extern Ndk::EntityHandle make_character(const Ndk::WorldHandle& w, const CharacterData& infos);

///
/// \fn make_item
///
/// \brief Creates an entity and adds it ItemComponent and NameComponent
///

extern Ndk::EntityHandle make_item(const Ndk::WorldHandle& w, const Nz::String& codename, 
                                   const Nz::String& name = "Item", const Nz::String& desc = "Empty description", unsigned level = 1);

//extern Ndk::EntityHandle make_mapObject(const Ndk::WorldHandle& w, const MapObjectData& infos);

#endif // FACTORY_HPP
