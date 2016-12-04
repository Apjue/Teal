// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPUTIL_HPP
#define MAPUTIL_HPP

#include <NDK/Entity.hpp>
#include <utility>
#include <memory>
#include "micropather.h"
#include "components/common/mapcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/mappositioncomponent.hpp"
#include "components/common/orientationcomponent.hpp"
#include "def/gamedef.hpp"
#include "global.hpp"
#include "data/mapdata.hpp"
#include "util/entityutil.hpp"

inline MapDataRef activateMapEntities(const MapDataRef& map);
inline MapDataRef deactivateMapEntities(const MapDataRef& map);

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

extern void initMapUtility(const std::weak_ptr<MapInstance>& currentMap,
                           const std::weak_ptr<micropather::MicroPather>& pather);

///
/// \fn isMapUtilityInited
///
/// \brief Checks if the MapCore and MapInstance pointers used by
///        functions in this files are valid
///

extern bool isMapUtilityInited();

inline Nz::String mapXYToString(int x, int y);
inline std::pair<int, int> stringToMapXY(const Nz::String& str);

#include "maputil.inl"

#endif // MAPUTIL_HPP
