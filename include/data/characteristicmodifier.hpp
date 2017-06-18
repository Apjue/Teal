// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CHARACTERISTICMODIFIER_HPP
#define CHARACTERISTICMODIFIER_HPP

#include "attack.hpp"

struct CharacteristicModifier : public Attack
{
    int movementPoints {};
    int actionPoints {};
};

#endif // CHARACTERISTICMODIFIER_HPP
