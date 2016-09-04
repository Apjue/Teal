// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef POSREFRESHSYSTEM_HPP
#define POSREFRESHSYSTEM_HPP

#include <NDK/System.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include "components.hpp"
#include "gamedef.hpp"
#include "util.hpp"

///
/// \class PosRefreshSystem
///
/// \brief Refresh the graphics position of items
///        using their logic position
///
/// \note It is a workaround
///       [WORKAROUND 2]
///

class PosRefreshSystem : public Ndk::System<PosRefreshSystem>
{
public:
    PosRefreshSystem()
    {
        Requires<PositionComponent, DefaultGraphicsPosComponent,
            Ndk::GraphicsComponent, Ndk::NodeComponent>();
    }
    ~PosRefreshSystem() = default;

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float elapsed) override;
};

#endif // POSREFRESHSYSTEM_HPP