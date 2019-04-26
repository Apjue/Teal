// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef MOVEMENTSYSTEM_HPP
#define MOVEMENTSYSTEM_HPP

#include <NDK/System.hpp>
#include <NDK/Entity.hpp>

///
/// \class MovementSystem
///
/// \brief Moves any entity which has a path and a position component.
///        If the path is empty, it does nothing
///        The move needs severals calls to update to be fully done
///

class MovementSystem : public Ndk::System<MovementSystem>
{
public:
    MovementSystem() = delete;
    MovementSystem(const Ndk::EntityHandle& mainCharacter);
    ~MovementSystem() = default;

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float elapsed) override;

    Ndk::EntityHandle m_mainCharacter;
};


#endif // MOVEMENTSYSTEM_HPP
