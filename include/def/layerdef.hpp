// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LAYERDEF_HPP
#define LAYERDEF_HPP

namespace Def
{

constexpr int MapLayer { 0 };
constexpr int MapObjectsLayer { MapLayer + 1 }; // Map objects, e.g. box
constexpr int MapItemsLayer { MapObjectsLayer + 1 }; // Items on the ground
constexpr int CharactersLayer { MapItemsLayer + 1 }; // Player, NPCs
constexpr int ItemsLayer { CharactersLayer + 1 }; // Items equipped by the character, in the map
constexpr int PauseMenuBackgroundLayer { ItemsLayer + 1 };
constexpr int PauseMenuButtonsLayer { PauseMenuBackgroundLayer + 1 };

}

#endif // LAYERDEF_HPP
