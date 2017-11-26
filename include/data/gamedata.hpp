// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#include <NDK/Entity.hpp>
#include <NDK/EntityList.hpp>
#include "cache/tilesetcore.hpp"
#include "cache/doublestore.hpp"

struct GameData
{
    TilesetCore tilesetCore;
    TilesetCore fightTilesetCore;

    StateMDStore states;
    EffectMDStore effects;
    SkillStore skills;
    AnimationStore animations;

    Ndk::EntityList items;
    Ndk::EntityList characters;
};

#endif // GAMEDATA_HPP
