// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef FIGHTCOMPONENT_HPP
#define FIGHTCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Core/ObjectHandle.hpp>
#include <vector>
#include "components/characters/pathcomponent.hpp"
#include "cache/doublestore.hpp"
#include "global.hpp"
#include "util/nzstlcompatibility.hpp"

struct FightComponent;
using FightComponentHandle = Nz::ObjectHandle<FightComponent>;

struct FightComponent : public Ndk::Component<FightComponent>
{
    FightComponent(bool autoAttack = false, unsigned mov = 3, unsigned action = 6)
        : automaticallyAttack { autoAttack }, maxMovementPoints { mov }, maxActionPoints { action }, movementPoints { mov }, actionPoints { action } {}

    bool isFighting { false };
    bool myTurn { false }; // Waiting to kill you
    unsigned teamNumber {};
    
    bool automaticallyAttack { false }; // In the map

    unsigned maxMovementPoints { 3 };
    unsigned maxActionPoints { 6 };

    unsigned movementPoints { maxMovementPoints };
    unsigned actionPoints { maxActionPoints };

    std::vector<SkillStore::LightId> attacks;
    std::vector<StateData> states; // States which don't remain after fight, for others, see StateComponent

    SkillStore::LightId selectedAttack { SkillStore::InvalidID };
    Ndk::EntityHandle target;

    static Ndk::ComponentIndex componentIndex;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, FightComponent* component, TypeTag<FightComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);

    component->isFighting = state.CheckField<bool>("is_fighting", false, index);
    component->myTurn = state.CheckField<bool>("my_turn", false, index);
    component->teamNumber = state.CheckField<unsigned>("team_number", index);

    component->automaticallyAttack = state.CheckField<bool>("automatically_attack", false, index);

    component->maxMovementPoints = state.CheckField<unsigned>("max_movement_points", index);
    component->maxActionPoints = state.CheckField<unsigned>("max_action_points", index);

    component->movementPoints = state.CheckField<unsigned>("movement_points", index);
    component->actionPoints = state.CheckField<unsigned>("action_points", index);

    component->attacks = state.CheckField<std::vector<SkillStore::LightId>>("attacks", index);
    component->states = state.CheckField<std::vector<StateData>>("states", index);

    return 1;
}

inline int LuaImplReplyVal(const LuaState& state, FightComponentHandle&& component, TypeTag<FightComponentHandle>)
{
    state.PushTable();
    {
        state.PushField("is_fighting", component->isFighting);
        state.PushField("my_turn", component->myTurn);
        state.PushField("team_number", component->teamNumber);

        state.PushField("automatically_attack", component->automaticallyAttack);

        state.PushField("max_movement_points", component->maxMovementPoints);
        state.PushField("max_action_points", component->maxActionPoints);

        state.PushField("movement_points", component->movementPoints);
        state.PushField("action_points", component->actionPoints);

        state.PushField("attacks", component->attacks);
        state.PushField("states", component->states);
    }

    return 1;
}

} // namespace Nz

#endif // FIGHTCOMPONENT_HPP
