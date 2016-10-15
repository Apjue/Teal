// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UTIL_HPP
#define UTIL_HPP

#include <Nazara/Graphics/Sprite.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <Nazara/Core/Color.hpp>
#include <Nazara/Utility/Image.hpp>

#include <cassert>
#include <array>
#include "def/gamedef.hpp"
#include "global.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/defaultgraphicsposcomponent.hpp"

inline unsigned dToU(double d);

template<class T>
inline T distance(const T& x, const T& y);

inline DiffTile AbsPosToDiff(const AbsTile& from, const AbsTile& to);

#include "util.inl"

#endif // UTIL_HPP

