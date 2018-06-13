// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEALINIT_HPP
#define TEALINIT_HPP

#include <NDK/World.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include "data/gamedata.hpp"

void initializeTeal(Ndk::World& world, Nz::RenderWindow& window, GameData& data);
void uninitializeTeal(GameData& data);

namespace Detail
{

// Init Functions
void loadTextures();
void loadNazara();
void initSchemeUtility();
void loadTilesetCore(TilesetCore& tilesetCore, TilesetCore& fightTilesetCore);

void loadMetaData(StateMDStore& states);
void loadSkills(SkillStore& skills);
void loadFightAIs(AICore& ais);
void loadAnimations(AnimationStore& animations);
void loadCharacters(Ndk::World& world, Ndk::EntityList& characters, AnimationStore& animations);
void loadItems(Ndk::World& world, Ndk::EntityList& items, const SkillStore& skills);
//void loadMapObjects();
void loadMaps(const Ndk::EntityList& characters, const Ndk::EntityList& items);

void addIcon(Nz::RenderWindow& window);
void addCam(Ndk::World& world, Nz::RenderWindow& window);

}

#endif // TEALINIT_HPP
