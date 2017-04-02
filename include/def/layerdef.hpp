﻿// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LAYERDEF_HPP
#define LAYERDEF_HPP

namespace Def
{

constexpr int MAP_LAYER { 0 };
constexpr int CHARACTERS_LAYER { MAP_LAYER + 1 }; // Player, NPCs, mobs
constexpr int ITEMS_LAYER { CHARACTERS_LAYER + 1 }; // Items equipped by the character, in the map
constexpr int PAUSE_MENU_BACKGROUND_LAYER { ITEMS_LAYER + 1 };
constexpr int PAUSE_MENU_BUTTONS_LAYER { PAUSE_MENU_BACKGROUND_LAYER + 1 };

}

#endif // LAYERDEF_HPP
