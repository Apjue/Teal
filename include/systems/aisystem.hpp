// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef AISYSTEM_HPP
#define AISYSTEM_HPP

#include <NDK/System.hpp>
#include "components/common/pathcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/movetocomponent.hpp"
#include "components/common/mapcomponent.hpp"
#include "micropather.h"

///
/// \class AISystem
///
/// \brief Compute a path using the Position and MoveTo component
///        And write it to the Path component.
///
/// \note It uses micropather
///

class AISystem : public Ndk::System<AISystem>
{
public:
    AISystem()
    {
        Requires<PathComponent, PositionComponent, MoveToComponent>();
    }
    AISystem(const std::shared_ptr<micropather::MicroPather>& pather)
        : AISystem()
    {
        setPather(pather);
    }
    ~AISystem() = default;

    inline void reset()
    {
        NazaraAssert(m_pather, "Pather is null !");
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

#endif // AISYSTEM_HPP