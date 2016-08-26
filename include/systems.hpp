// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <NDK/World.hpp>
#include <NDK/System.hpp>
#include <NDK/Systems.hpp>
#include <NDK/EntityOwner.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <assert.h>
#include <utility>

#include "basesystems.hpp"
#include "components.hpp"
#include "global.hpp"
#include "util.hpp"
#include "chrono.hpp"

namespace Systems
{

///
/// \class AnimationSystem
///
/// \brief Animates Graphics Items, as long as
///        their type is QGraphicsPixmapItem
///
/// 

class AnimationSystem : public Ndk::System<AnimationSystem>
{
public:
    AnimationSystem()
    {
        Requires<Components::Animation, Ndk::GraphicsComponent, Components::Position, Components::CDirection>();
        SetUpdateRate(Def::MAXFPS);
    }
    ~AnimationSystem() = default;

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float elapsed) override;

    void UndefinedStateAnimation(); //used to log errors
    void MovingStateAnimation(unsigned startX, unsigned startY, Nz::SpriteRef gfx,
                              Components::Animation& anim, bool moving);
};

///
/// \brief The PosRefreshSystem class
///
/// Refresh the graphics position of items
/// using their logic position
/// [WORKAROUND 2]
///

class PosRefreshSystem : public Ndk::System<PosRefreshSystem>
{
public:
    PosRefreshSystem()
    {
        Requires<Components::Position, Ndk::GraphicsComponent, Ndk::NodeComponent>();
    }
    ~PosRefreshSystem() = default;

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float elapsed) override;
};

///
/// \brief The AISystem class
///
/// Compute a path with the Position and MoveTo component
/// And write it to the Path component.
/// (uses micropather)
///

class AISystem : public Ndk::System<AISystem>
{
public:
    AISystem()
    {
        Requires<Components::Path, Components::Position, Components::MoveTo>();
    }
    AISystem(const std::shared_ptr<micropather::MicroPather>& pather)
        : AISystem()
    {
        setPather(pather);
    }
    ~AISystem() = default;

    inline void reset()
    {
        assert(m_pather && "Pather is null !");
        m_pather->Reset();
    }
    void setPather(const std::shared_ptr<micropather::MicroPather>& pather)
    {
        m_pather = pather;
    }

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float elapsed) override;

    std::shared_ptr<micropather::MicroPather> m_pather {};
};

///
/// \brief The MovementSystem class
///
/// Moves any entity which has a path component
/// and a position component.
/// If the path is empty, no move.
/// The move isn't done with one call to update()
///

class MovementSystem : public Ndk::System<MovementSystem>
{
public:
    MovementSystem()
    {
        Requires<Components::Path, Components::Position, Components::CDirection>();
        SetUpdateRate(Def::MAXFPS);
    }
    ~MovementSystem() = default;

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float elapsed) override;
};

}

#endif // SYSTEMS_HPP
