// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEALINIT_HPP
#define TEALINIT_HPP

#include <NDK/World.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>

void initializeTeal(Ndk::World& world, Nz::RenderWindow& window);

namespace Detail
{

// Init Functions
void loadTextures();
void loadNazara();
void initSchemeUtility();
void loadTilesetCore();

void loadMetaData();
void loadSkills();
void loadAnimations();
void loadCharacters();
void loadItems();
//void loadMapObjects();
void loadMaps();

void addIcon(Nz::RenderWindow& window);
void addCam(Ndk::World& world, Nz::RenderWindow& window);

}

#endif // TEALINIT_HPP
