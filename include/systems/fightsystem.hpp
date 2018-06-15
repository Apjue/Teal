// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef FIGHTSYSTEM_HPP
#define FIGHTSYSTEM_HPP

#include <NDK/System.hpp>

class FightSystem : public Ndk::System<FightSystem>
{
public:
    FightSystem();
    ~FightSystem() = default;

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float) override;
};

#endif // FIGHTSYSTEM_HPP
