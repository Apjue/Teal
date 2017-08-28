// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MAPPOSUTIL_HPP
#define MAPPOSUTIL_HPP

#include <Nazara/Prerequesites.hpp>
#include <utility>
#include <cstdint>
#include "util/assert.hpp"
#include "util/util.hpp"
#include "def/gamedef.hpp"
#include "global.hpp"

inline void  NodeToXY(void* node, unsigned& x, unsigned& y);
inline void* XYToNode(unsigned x, unsigned y);

inline std::pair<unsigned, unsigned> IndexToXY(unsigned index);
inline unsigned XYToIndex(unsigned x, unsigned y);

inline std::pair<unsigned, unsigned> countLines(unsigned y);

#include "mapposutil.inl"

#endif // MAPPOSUTIL_HPP
