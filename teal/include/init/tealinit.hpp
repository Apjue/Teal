// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEALINIT_HPP
#define TEALINIT_HPP

#include <NDK/World.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include "data/gamedata.hpp"

void initializeTeal(GameData& data);
void uninitializeTeal(GameData& data);

namespace TealInitDetail
{

// Init Functions
void loadTextures();
void loadNazara();
void initSchemeUtility();
void loadTilesetCore(TilesetCore& tilesetCore, TilesetCore& fightTilesetCore);

void loadStatesMetaData(StateMDStore& states);
void loadSkills(SkillStore& skills);
void loadFightAIs(AICore& ais);
void loadAnimations(AnimationStore& animations);
void loadCharacters(Ndk::WorldHandle world, Ndk::EntityList& characters);
void loadMonsters(Ndk::WorldHandle world, Ndk::EntityList& monsters);
void loadItems(Ndk::WorldHandle world, Ndk::EntityList& items, const SkillStore& skills);
void loadMapObjects(Ndk::WorldHandle world, Ndk::EntityList& mapObjects);
void loadMaps(Ndk::WorldHandle world, const GameData& gameData);

void addIcon(Nz::RenderWindow& window);
void addCam(Ndk::WorldHandle world, Nz::RenderWindow& window);

}

#endif // TEALINIT_HPP
