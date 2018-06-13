// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RANDOMMOVEMENTSYSTEM_HPP
#define RANDOMMOVEMENTSYSTEM_HPP

#include <NDK/System.hpp>
#include <memory>
#include "components/common/mapcomponent.hpp"
#include "util/random.hpp"

class RandomMovementSystem : public Ndk::System<RandomMovementSystem>
{
public:
    RandomMovementSystem();
    RandomMovementSystem(const std::shared_ptr<micropather::MicroPather>& pather);
    ~RandomMovementSystem() = default;

    void setPather(const std::shared_ptr<micropather::MicroPather>& pather);

    static Ndk::SystemIndex systemIndex;

private:
    std::uniform_int_distribution<unsigned> m_uni;
    std::shared_ptr<micropather::MicroPather> m_pather;

    void OnUpdate(float elapsed) override;
};

#endif // RANDOMMOVEMENTSYSTEM_HPP
