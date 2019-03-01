// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_CHARACTERDATA_HPP
#define TEAL_CHARACTERDATA_HPP

#include "data/livingentitydata.hpp"

struct CharacterData // Contains all required attributes of a character. Used by a character factory.
{
    struct RandomMovement
    {
        RandomMovement(bool enable = false, float mov = 7.5f, unsigned nb = 1u)
            : enabled { enable }, movementInterval { mov }, range { nb }
        {}

        bool enabled;
        float movementInterval;
        unsigned range;
    };

    CharacterData() = default;
    ~CharacterData() = default;

    LivingEntityData livingEntityData;

    RandomMovement randomMovement;
    bool blockTile;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, CharacterData::RandomMovement* random, TypeTag<CharacterData::RandomMovement>);
inline unsigned int LuaImplQueryArg(const LuaState& state, int index, CharacterData* data, TypeTag<CharacterData>);

} // namespace Nz

#include "characterdata.inl"

#endif // TEAL_CHARACTERDATA_HPP
