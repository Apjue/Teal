// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATEDATA_HPP
#define STATEDATA_HPP

#include "cache/doublestore.hpp"
#include "attack.hpp"

struct StateData : public Attack
{
    StateStore::LightId stateId {};
};

#endif // STATEDATA_HPP
