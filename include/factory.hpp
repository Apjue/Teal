// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <NDK/World.hpp>
#include <NDK/Entity.hpp>
#include "components.hpp"
#include "util.hpp"
#include "characterinfos.hpp"

///
/// \brief The make_character function
///
/// Creates a character with all required components
///
/// \note There's no difference between a character and a NPC
///

extern Ndk::EntityHandle make_character(Ndk::WorldHandle& w, const CharacterInfos& c);

#endif // FACTORY_HPP
