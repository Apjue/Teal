// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef BASESYSTEMS_HPP
#define BASESYSTEMS_HPP

#include <NDK/World.hpp>

namespace Systems
{

///
/// \brief The BaseRenderSystem class
///
/// Base class of all render systems
/// TODO: delete ?
///

class BaseRenderSystem
{

public:
    BaseRenderSystem() = default;
    ~BaseRenderSystem() = default;

    void setWorld(const Ndk::WorldHandle& world) noexcept
    {
        m_world = world;
    }
    inline Ndk::WorldHandle& getWorld() noexcept
    {
        assert(isValid() && "Render system is not valid");
        return m_world;
    }
    virtual inline bool isValid() const
    {
        return m_world.IsValid();
    }

private:
    Ndk::WorldHandle m_world {};
};

}

#endif // BASESYSTEMS_HPP
