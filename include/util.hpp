// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UTIL_HPP
#define UTIL_HPP

#include <Nazara/Graphics/Sprite.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <Nazara/Core/Color.hpp>
#include <Nazara/Utility/Image.hpp>

#include <assert.h>
#include <array>
#include "chrono.hpp"
#include "non_nullptr.hpp"
#include "gamedef.hpp"
#include "global.hpp"

inline unsigned dToU(double d)
{
    return static_cast<unsigned>(d);
}

template<class T>
inline T distance(const T& x, const T& y)
{
    return (x > y) ? x - y : y - x;
}

///
/// \fn getSpriteFromComponent
///
/// \brief Returns the first sprite it found in the Graphics Component
///        or nullptr if it didn't found one
///
/// \param gfx Graphics Component to search into
///

extern Nz::Sprite* getSpriteFromComponent(Ndk::GraphicsComponent& gfx);

///
/// \fn getTileFromGlobalCoords
///
/// \brief Returns the tile located at the x, y position
///
/// \param e Global Coordinates of the tile
///
/// \returns The logic position of the tile
///
/// \todo Add the z axis ?
///

extern AbsTile getTileFromGlobalCoords(const Nz::Vector2ui& coords);

// extern QJsonDocument jsonFromFile(const QString& filename);
// extern void jsonToFile(const QJsonDocument& document, const QString& filename);

#endif // UTIL_HPP
