// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPUTIL_HPP
#define MAPUTIL_HPP

#include <NDK/Entity.hpp>
#include <utility>

#include "cache/mapcore.hpp"
#include "components/common/mapcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/mappositioncomponent.hpp"
#include "def/gamedef.hpp"
#include "global.hpp"

///
/// \fn hasComponentsToChangeMap
///
/// \brief Checks if the entity has the components to change a map
///        i.e. when entity has PositionComponent and MapPositionComponent.
///
/// \param p Entity to check
///

inline bool hasComponentsToChangeMap(const Ndk::EntityHandle& p);

///
/// \fn canChangeMap
///
/// \brief Checks if the MapCore contains the map the entity will go
///        And if there is no obstacle at the position where the entity
///        will be when changing map
///
/// \note The map where the entity will go is determined by the position
///       of the entity
///
/// \return A pair with - a bool      -> If the entity can change map
///                     - a direction -> If yes, which map the entity can move to
///

extern std::pair<bool, Direction::Dir> canChangeMap(const Ndk::EntityHandle& p);

///
/// \fn changeMap
///
/// \return If the entity changed map correctly
///

extern bool changeMap(const Ndk::EntityHandle& p);

///
/// \fn initMapUtility
///
/// \brief Inits MapCore and MapInstance for functions
///        in this file to work
///

extern void initMapUtility(MapCore* maps, const std::shared_ptr<MapInstance>& currentMap);

///
/// \fn isMapUtilityInited
///
/// \brief Checks if the MapCore and MapInstance pointers used by
///        functions in this files are valid
///

extern bool isMapUtilityInited();

#include "maputil.inl"

#endif // MAPUTIL_HPP
