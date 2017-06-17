// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ATTACK_HPP
#define ATTACK_HPP

#include "attackdata.hpp"

struct Attack
{
    Attack() = default;
    virtual ~Attack() = default;

    AttackData data;
};

#endif // ATTACK_HPP
