// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef AISYSTEM_HPP
#define AISYSTEM_HPP

#include <NDK/System.hpp>
#include "components/common/pathcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/movecomponent.hpp"
#include "components/common/fightcomponent.hpp"
#include "components/common/lifecomponent.hpp"
#include "util/aiutil.hpp"
#include "micropather.h"

///
/// \class AISystem
///
/// \brief Compute a path using the Position and Move component
///        And write it to the Path component.
///
/// \note It uses micropather
///

class AISystem : public Ndk::System<AISystem>
{
public:
    AISystem();
    AISystem(const std::shared_ptr<micropather::MicroPather>& pather);
    ~AISystem() = default;

    void reset();
    void setPather(const std::shared_ptr<micropather::MicroPather>& pather);

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float elapsed) override;

    std::shared_ptr<micropather::MicroPather> m_pather {};
};

#endif // AISYSTEM_HPP
