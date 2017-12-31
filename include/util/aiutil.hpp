// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef AIUTIL_HPP
#define AIUTIL_HPP

#include <NDK/Entity.hpp>
#include "components/common/pathcomponent.hpp"
#include "micropather.h"

PathComponent::PathPool computePath(const Ndk::EntityHandle& e, micropather::MicroPather* pather);

PathComponent::PathPool computePath(const AbsTile& startPosition, const AbsTile& lastPosition, micropather::MicroPather* pather);

#endif // AIUTIL_HPP
