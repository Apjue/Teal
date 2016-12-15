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
/// \todo Add bio component or description component
///

extern Ndk::EntityHandle make_item(const Ndk::WorldHandle& w, const Nz::String& name = "Item");

#endif // FACTORY_HPP
