// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef AISYSTEM_HPP
#define AISYSTEM_HPP

#include <Nazara/Lua/LuaCoroutine.hpp>
#include <Nazara/Lua/LuaInstance.hpp>
#include <Nazara/Core/String.hpp>
#include <NDK/System.hpp>
#include <NDK/EntityList.hpp>
#include <vector>
#include <memory>
#include <unordered_map>
#include "micropather.h"
#include "cache/doublestore.hpp"

namespace Detail
{

struct FightData
{
    bool clean { true };
    Nz::LuaCoroutine* coroutine {};
    Nz::LuaInstance ai;

    Ndk::EntityHandle currentEntity;
    std::vector<Ndk::EntityHandle> fighters;
    Ndk::EntityList entities; // Others entities: Traps, unanimated objects, ...
};

} // namespace Detail

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
    AISystem(const SkillStore& skills, const Nz::String& utilFilepath, const std::shared_ptr<micropather::MicroPather>& pather, const Ndk::EntityHandle& mainCharacter);
    AISystem(const AISystem& other);
    ~AISystem() = default;

    void reset();
    void setPather(const std::shared_ptr<micropather::MicroPather>& pather);

    static Ndk::SystemIndex systemIndex;

private:
    void OnUpdate(float elapsed) override;

    void cleanAndContinueFight();
    void cleanLuaInstance(Nz::LuaInstance& lua);

    bool prepareLuaAI(Nz::LuaInstance& lua);
    bool serializeCharacter(Nz::LuaInstance& lua, const Ndk::EntityHandle& character, bool skills = true);
    bool serializeSkills(Nz::LuaInstance& lua, const Ndk::EntityHandle& character);

    void Teal_MoveCharacter(unsigned x, unsigned y);
    void Teal_TakeCover();
    void Teal_AttackCharacter(unsigned characterIndex, Nz::String skillCodename);
    void Teal_MoveAndAttackCharacter(unsigned characterIndex, Nz::String skillCodename);
    unsigned Teal_ChooseTarget();
    unsigned Teal_ChooseAttack(unsigned characterIndex);
    bool Teal_CanAttack(unsigned characterIndex);

    std::shared_ptr<micropather::MicroPather> m_pather {};
    Ndk::EntityHandle m_mainCharacter;
    Nz::String m_utilityLuaFile;

    Detail::FightData m_currentFight;
    bool m_isFightActive {};

    const SkillStore& m_skills;
};

#endif // AISYSTEM_HPP
