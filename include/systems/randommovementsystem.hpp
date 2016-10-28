// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RANDOMMOVEMENTSYSTEM_HPP
#define RANDOMMOVEMENTSYSTEM_HPP

#include <NDK/System.hpp>
#include <random>
#include "components/common/positioncomponent.hpp"
#include "components/common/movetocomponent.hpp"
#include "components/common/randommovementcomponent.hpp"
#include "util/random.hpp"
#include "global.hpp"

class RandomMovementSystem : public Ndk::System<RandomMovementSystem>
{
public:
    RandomMovementSystem();
    ~RandomMovementSystem() = default;

    static Ndk::SystemIndex systemIndex;

private:
    std::uniform_int_distribution<unsigned> m_uni;

    void OnUpdate(float elapsed) override;
};

#endif // RANDOMMOVEMENTSYSTEM_HPP
