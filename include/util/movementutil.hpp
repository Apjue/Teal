// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MOVEMENTUTIL_HPP
#define MOVEMENTUTIL_HPP

#include <NDK/Entity.hpp>
#include "components/common/pathcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/orientationcomponent.hpp"
#include "util/gfxutil.hpp"
#include "def/gamedef.hpp"
#include "util/maputil.hpp"
#include "util/entityutil.hpp"

void moveEntity(const Ndk::EntityHandle& e);

#endif // MOVEMENTUTIL_HPP
