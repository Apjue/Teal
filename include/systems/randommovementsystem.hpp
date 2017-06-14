// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RANDOMMOVEMENTSYSTEM_HPP
#define RANDOMMOVEMENTSYSTEM_HPP

#include <NDK/System.hpp>
#include <random>
#include <memory>
#include <unordered_set>
#include <algorithm>
#include "components/common/mapcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/movecomponent.hpp"
#include "components/common/randommovementcomponent.hpp"
#include "util/random.hpp"
#include "global.hpp"
#include "util/assert.hpp"

class RandomMovementSystem : public Ndk::System<RandomMovementSystem>
{
public:
    RandomMovementSystem();
    RandomMovementSystem(const std::weak_ptr<MapInstance>& map);
    ~RandomMovementSystem() = default;

    void setMap(const std::weak_ptr<MapInstance>& map);

    static Ndk::SystemIndex systemIndex;

private:
    std::uniform_int_distribution<unsigned> m_uni;
    std::weak_ptr<MapInstance> m_map;

    void OnUpdate(float elapsed) override;
};

#endif // RANDOMMOVEMENTSYSTEM_HPP
