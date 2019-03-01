// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include <NDK/World.hpp>
#include <NDK/Entity.hpp>
#include <NDK/EntityList.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <memory>
#include "cache/tilesetcore.hpp"
#include "cache/doublestore.hpp"
#include "cache/aicore.hpp"

struct GameData
{
    Ndk::WorldHandle world;
    Nz::RenderWindow* window;

    TilesetCore tilesetCore;
    TilesetCore fightTilesetCore;

    std::shared_ptr<StateMDStore>   states; // Wondering why shared_ptr are used?
    std::shared_ptr<SkillStore>     skills; // Check doublestore.hpp
    std::shared_ptr<AnimationStore> animations;
    AICore ais;

    Ndk::EntityList items;
    Ndk::EntityList characters;
    Ndk::EntityList monsters;
};

#endif // GAMEDATA_HPP
