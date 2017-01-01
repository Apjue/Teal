// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ENTITYUTIL_HPP
#define ENTITYUTIL_HPP

#include <NDK/Entity.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include "components/common/defaultgraphicsposcomponent.hpp"
#include "components/common/mappositioncomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/items/itemcomponent.hpp"

///
/// \fn isMapEntity
///
/// \brief Returns true if the entity is a map object (e.g. a tree or a NPC)
///        (i.e. has GraphicsComponent, NodeComponent, DefaultGraphicsPosComponent)
///

inline bool isMapEntity(const Ndk::EntityHandle& e);

///
/// \fn hasComponentsToChangeMap
///
/// \brief Checks if the entity has the components to change a map
///        (i.e. when entity has PositionComponent and MapPositionComponent)
///
/// \todo Change name of function to isXXX ?
///

inline bool hasComponentsToChangeMap(const Ndk::EntityHandle& e);

///
/// \fn isItemEntity
///
/// \return If the entity is an item
///         (i.e. if it has the ItemComponent)
///

inline bool isItemEntity(const Ndk::EntityHandle& e);

#include "entityutil.inl"

#endif // ENTITYUTIL_HPP
