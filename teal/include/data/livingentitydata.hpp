// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_LIVINGENTITYDATA_HPP
#define TEAL_LIVINGENTITYDATA_HPP

#include <Nazara/Core/String.hpp>
#include "components/characters/animationcomponent.hpp"
#include "components/shared/damagemodifiercomponent.hpp"
#include "data/mapentitydata.hpp"
#include "data/elementdata.hpp"
#include "containers/doublestore.hpp"
#include "util/util.hpp"
#include "global.hpp"

struct LivingEntityData // Contains all required attributes of a living entity. Used by a living entity factory.
{
    struct Fight
    {
        Fight(bool f = false, bool autoAtk = false, unsigned mov = 3, unsigned action = 6)
            : fight { f }, autoAttack { autoAtk }, movementPoints { mov }, actionPoints { action } {}

        bool fight;
        bool autoAttack;
        unsigned movementPoints;
        unsigned actionPoints;
    };

    MapEntityData mapEntityData;

    AnimationComponent::AnimationList animations;

    unsigned maxHp;
    Orientation orientation;

    Nz::String name;
    Nz::String description;

    DamageModifierList damageData;

    unsigned level;
    Fight fight;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, LivingEntityData::Fight* fight, TypeTag<LivingEntityData::Fight>);
inline unsigned int LuaImplQueryArg(const LuaState& state, int index, LivingEntityData* data, TypeTag<LivingEntityData>);

} // namespace Nz

#include "livingentitydata.inl"

#endif // TEAL_LIVINGENTITYDATA_HPP
