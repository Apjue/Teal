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
#include "util.hpp"
#include "layerdef.hpp"
#include "data/characterdata.hpp"

///
/// \fn make_character
///
/// \brief Creates a character with all required components
///
/// \note There's no difference between a player and a NPC
///       But only NPCs have the RandomMovement component
///

extern Ndk::EntityHandle make_character(Ndk::WorldHandle& w, const CharacterData& infos);

#endif // FACTORY_HPP
