// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_ENTITYFACTORY_HPP
#define TEAL_ENTITYFACTORY_HPP

#include <NDK/World.hpp>
#include <NDK/Entity.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <Nazara/Lua/LuaInstance.hpp>
#include "data/characterdata.hpp"
#include "data/monsterdata.hpp"
#include "data/graphicalitemdata.hpp"
//#include "data/mapobjectdata.hpp"

extern Ndk::EntityHandle makeLivingEntity(const Ndk::WorldHandle& w, const LivingEntityData& data);
extern Ndk::EntityHandle makeCharacter(const Ndk::WorldHandle& w, const CharacterData& data);
extern Ndk::EntityHandle makeMonster(const Ndk::WorldHandle& w, const MonsterData& data);
extern Ndk::EntityHandle makeMonsterGroup(const Ndk::WorldHandle& w, float interval, unsigned range);


extern Ndk::EntityHandle makeLogicalItem(const Ndk::WorldHandle& w, Nz::String codename, Nz::String name = "Item", Nz::String description = "Empty description",
                                         unsigned level = 1, Nz::TextureRef icon = Nz::TextureLibrary::Get(":/game/unknown"), Nz::Vector2f mapOffset = {});
extern Ndk::EntityHandle makeLogicalItem(const Ndk::WorldHandle& w, Nz::LuaInstance& lua);

extern Ndk::EntityHandle makeGraphicalItem(Ndk::EntityHandle e, const GraphicalItemData& data);
extern Ndk::EntityHandle makeGraphicalItem(const Ndk::WorldHandle& w, const GraphicalItemData& data);
extern Ndk::EntityHandle makeGraphicalItem(const Ndk::WorldHandle& w, const GraphicalItemData& data, AbsTile pos); // Insert in a map the graphical item, not the logic item !

//extern Ndk::EntityHandle make_mapObject(const Ndk::WorldHandle& w, const MapObjectData& infos);

#endif // TEAL_ENTITYFACTORY_HPP
