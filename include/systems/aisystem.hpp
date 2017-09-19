// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef AISYSTEM_HPP
#define AISYSTEM_HPP

#include <Nazara/Lua/LuaInstance.hpp>
#include <NDK/System.hpp>
#include <NDK/EntityList.hpp>
#include <memory>
#include <unordered_map>
#include "micropather.h"

namespace detail
{

struct FightData
{
    Ndk::EntityList fighters;
    Ndk::EntityList entities; // Others entities: Traps, unanimated objects, ...
};

}

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
    bool prepareLuaAI(Nz::LuaInstance& lua, const Ndk::EntityHandle& character, const detail::FightData& fight);

    std::shared_ptr<micropather::MicroPather> m_pather {};

    detail::FightData m_currentFight;
    bool m_isFightActive {};
};

#endif // AISYSTEM_HPP
