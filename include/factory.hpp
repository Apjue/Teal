// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <NDK/World.hpp>
#include <NDK/Entity.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include "data/characterdata.hpp"
//#include "data/mapobjectdata.hpp"

extern Ndk::EntityHandle make_character(const Ndk::WorldHandle& w, const CharacterData& infos);


extern Ndk::EntityHandle make_logicalItem(const Ndk::WorldHandle& w, const Nz::String& codename, const Nz::String& name = "Item",
                                          const Nz::String& desc = "Empty description", unsigned level = 1, Nz::TextureRef icon = {});

extern Ndk::EntityHandle make_mapItem(const Ndk::WorldHandle& w, const Ndk::EntityHandle& logicItem, const Nz::Vector2f& size, const Nz::Vector2f& defGfxPos, int renderOrder = 0);
extern Ndk::EntityHandle make_graphicalItem(const Ndk::WorldHandle& w, const Ndk::EntityHandle& logicItem, const Nz::Vector2f& size, const Nz::Vector2f& defGfxPos, int renderOrder = 0);

//extern Ndk::EntityHandle make_mapObject(const Ndk::WorldHandle& w, const MapObjectData& infos);

#endif // FACTORY_HPP
