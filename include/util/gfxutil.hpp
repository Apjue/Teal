// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GFXUTIL_HPP
#define GFXUTIL_HPP

#include <NDK/Components/GraphicsComponent.hpp>
#include <Nazara/Utility/Image.hpp>
#include "components/util/renderablesstoragecomponent.hpp"
#include "global.hpp"

inline void cloneRenderables(const Ndk::EntityHandle& e, unsigned renderOrder = 0);
extern void cloneRenderables(Ndk::GraphicsComponent& gfx, RenderablesStorageComponent& renderables, unsigned renderOrder = 0);

///
/// \fn getTileFromGlobalCoords
///
/// \brief Returns the tile located at the coords position
///
/// \param coords Global Coordinates of the tile (like mouse position)
///
/// \returns The logic position of the tile
///
/// \note Needs a scheme set with the initSchemeUtility function
///
/// \todo Add the z axis ?
///

extern AbsTile getTileFromGlobalCoords(const Nz::Vector2ui& coords);

///
/// \fn initSchemeUtility
///
/// \brief Sets the scheme the getTileFromGlobalCoords function will use
///

extern void initializeSchemeUtility(const Nz::ImageRef& newScheme);

///
/// \fn refreshGraphicsPos
///
/// \brief Refreshes the Graphics Position of an entity containing
///        a Sprite attached to a GraphicsComponent, using the
///        Position component of a logical entity
///
/// \note For map entities ONLY.
///

extern void refreshGraphicsPos(const Ndk::EntityHandle& entity);

#include "gfxutil.inl"

#endif // GFXUTIL_HPP
