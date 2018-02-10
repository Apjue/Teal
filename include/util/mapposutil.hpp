// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPPOSUTIL_HPP
#define MAPPOSUTIL_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/Math/Rect.hpp>
#include <utility>
#include <cstdint>
#include "util/assert.hpp"
#include "util/util.hpp"
#include "def/gamedef.hpp"
#include "def/typedef.hpp"
#include "global.hpp"

inline void  NodeToXY(void* node, unsigned& x, unsigned& y);
inline void* XYToNode(unsigned x, unsigned y);

inline std::pair<unsigned, unsigned> IndexToXY(unsigned index);
inline unsigned XYToIndex(unsigned x, unsigned y);

inline std::pair<unsigned, unsigned> countLines(unsigned y);
inline unsigned distanceBetweenTiles(const AbsTile& a, const AbsTile& b);
inline float graphicalDistanceBetweenTiles(const AbsTile& a, const AbsTile& b);

inline Nz::Rectf getTileAABB(const AbsTile& tile);
inline Nz::Rectf getTileAABB(unsigned x, unsigned y);
inline Nz::Vector2f getTileCenter(const AbsTile& tile);
inline Nz::Vector2f getTileCenter(unsigned x, unsigned y);

inline Vector2fPair getTileCornerSegment(Orientation corner, const AbsTile& tile);
extern Vector2fPair getTileCornerSegment(Orientation corner, unsigned x, unsigned y);
extern Vector2fTriplet getTileOutterCorners(const AbsTile& from, const AbsTile& to);
inline Nz::Vector2f getTileVertex(Direction vertex, const AbsTile& tile);
extern Nz::Vector2f getTileVertex(Direction vertex, unsigned x, unsigned y);

#include "mapposutil.inl"

#endif // MAPPOSUTIL_HPP
