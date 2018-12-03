// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPUTIL_HPP
#define MAPUTIL_HPP

#include <NDK/Entity.hpp>
#include <Nazara/Core/String.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <utility>
#include <queue>
#include <vector>
#include <set>
#include "micropather.h"
#include "components/common/mapcomponent.hpp"
#include "components/common/pathcomponent.hpp"
#include "util/assert.hpp"
#include "util/entityutil.hpp"
#include "def/gamedef.hpp"
#include "data/mapdata.hpp"
#include "global.hpp"

inline MapDataRef activateMapEntities(const MapDataRef& map);
inline MapDataRef deactivateMapEntities(const MapDataRef& map);

///
/// \fn canChangeMap
///
/// \brief Checks if the MapLibrary contains the map the entity will go
///        And if there is no obstacle at the position where the entity
///        will be when changing map
///
/// \note The map where the entity will go is determined by the position
///       of the entity
///
/// \return A pair with: - a bool      -> If the entity can go to next map
///                      - a direction -> If the bool is set to true, where the entity can move to
///

extern std::pair<bool, DirectionFlags> canChangeMap(const Ndk::EntityHandle& p);

///
/// \fn changeMap
///
/// \brief Change the game map and main character's map position
///
/// \return true if the entity changed map correctly
///

extern bool changeMap();

///
/// \fn initializeMapUtility
///
/// \brief Inits things for functions in this file to work
///

extern void initializeMapUtility(MapInstance* currentMap, micropather::MicroPather* pather, const Ndk::EntityHandle& mainCharacter);
inline void uninitializeMapUtility();

///
/// \fn isMapUtilityInitialized
///
/// \brief Checks if the MapInstance, Micropather, and main character pointers
///        used by functions in this files are valid
///

extern bool isMapUtilityInitialized();

extern Ndk::EntityList mapEntitiesHoveredByCursor(const Nz::Vector2ui& cursor);
extern void refreshOccupiedTiles();
extern void clearPatherCache();

extern const MapInstance* getCurrentMap();
extern const micropather::MicroPather* getPather();

extern std::vector<AbsTile> directionsToPositions(PathComponent::PathPool directions, AbsTile start);
extern std::set<AbsTile> getVisibleTiles(AbsTile pos, unsigned range, bool viewThroughObstacles = false, bool includeObstacles = false, bool removeOccupiedTiles = true);

#include "maputil.inl"

#endif // MAPUTIL_HPP
