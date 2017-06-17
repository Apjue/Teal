// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef DAMAGEDATA_HPP
#define DAMAGEDATA_HPP

#include <utility>
#include "elementdata.hpp"
#include "attack.hpp"

struct DamageData : public Attack
{
    std::pair<Element, int> damage;
};

#endif // DAMAGEDATA_HPP
