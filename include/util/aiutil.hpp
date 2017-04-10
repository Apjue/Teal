// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef AIUTIL_HPP
#define AIUTIL_HPP

#include <NDK/Entity.hpp>
#include <Nazara/Core/Error.hpp>
#include <vector>
#include "components/common/pathcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/movecomponent.hpp"
#include "components/common/mapcomponent.hpp"
#include "util/maputil.hpp"
#include "micropather.h"

std::queue<std::pair<DirectionFlags, bool>> computePath(const Ndk::EntityHandle& e, micropather::MicroPather* pather,
                                                        AbsTile* startPosition, AbsTile* lastPosition);

#endif // AIUTIL_HPP
