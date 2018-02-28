// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include <Nazara/Core/String.hpp>
#include "def/typedef.hpp"
#include "util/util.hpp"

struct Effect
{
    virtual ~Effect() = default;
    virtual void serialize(const Nz::LuaState& state) = 0;
};

struct MovementEffect : public Effect
{
    MovementEffect() = default;
    MovementEffect(const MovementEffect&) = default;
    MovementEffect(MovementEffect&&) = default;

    inline MovementEffect(const Nz::LuaState& state, int index = -1)
    {
        tiles = state.CheckField<unsigned>("tiles", index);
    }

    unsigned tiles {};

    virtual void serialize(const Nz::LuaState& state) override
    {
        state.PushField("tiles", tiles);
    }
};

    struct PullEffect : public MovementEffect
    {
        PullEffect() = default;
        PullEffect(const PullEffect&) = default;
        PullEffect(PullEffect&&) = default;

        inline PullEffect(const Nz::LuaState& state, int index = -1) : MovementEffect(state, index) {}

        static const char* getMetadataID() { return "pull"; }
    };

    struct PushEffect : public MovementEffect
    {
        PushEffect() = default;
        PushEffect(const PushEffect&) = default;
        PushEffect(PushEffect&&) = default;

        inline PushEffect(const Nz::LuaState& state, int index = -1) : MovementEffect(state, index) {}

        static const char* getMetadataID() { return "push"; }
    };

#endif // EFFECTS_HPP
