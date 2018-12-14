// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <NDK/World.hpp>
#include <NDK/Entity.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <Nazara/Lua/LuaInstance.hpp>
#include "components/util/logicentityidcomponent.hpp"
#include "data/characterdata.hpp"
#include "data/graphicalitemdata.hpp"
//#include "data/mapobjectdata.hpp"

extern Ndk::EntityHandle makeCharacter(const Ndk::WorldHandle& w, const CharacterData& data);
extern Ndk::EntityHandle makeCharacter(const Ndk::WorldHandle& w, Nz::LuaInstance& lua);

extern Ndk::EntityHandle makeLogicalItem(const Ndk::WorldHandle& w, const Nz::String& codename, const Nz::String& name = "Item",
                                         const Nz::String& description = "Empty description", unsigned level = 1, Nz::TextureRef icon = {});
extern Ndk::EntityHandle makeLogicalItem(const Ndk::WorldHandle& w, Nz::LuaInstance& lua);

extern Ndk::EntityHandle makeGraphicalItem(Ndk::EntityHandle e, const GraphicalItemData& data);
extern Ndk::EntityHandle makeGraphicalItem(const Ndk::WorldHandle& w, const GraphicalItemData& data);
extern Ndk::EntityHandle makeGraphicalItem(const Ndk::WorldHandle& w, const GraphicalItemData& data, AbsTile pos); // Insert in a map the graphical item, not the logic item !

//extern Ndk::EntityHandle make_mapObject(const Ndk::WorldHandle& w, const MapObjectData& infos);

#endif // FACTORY_HPP
