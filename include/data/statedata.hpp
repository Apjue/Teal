// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATEDATA_HPP
#define STATEDATA_HPP

#include <memory>
#include "attack.hpp"
#include "states.hpp"

struct StateData : public Attack
{
    std::shared_ptr<State> state;
};

#endif // STATEDATA_HPP
