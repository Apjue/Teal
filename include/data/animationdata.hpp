// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMATIONDATA_HPP
#define ANIMATIONDATA_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Renderer/Texture.hpp>

struct AnimationData
{
    enum AnimationType
    {
        Walk, // In map, normal animation
        Fight,
        Emote
    };

    static inline AnimationType stringToAnimationType(Nz::String string);
    static inline Nz::String animationTypeToString(AnimationType animType);

    AnimationType type {};
    Nz::Vector2ui size {};
    Nz::TextureRef texture; // Texture to apply on the sprite
    Nz::Vector2f offset {}; // offset to apply for the character to be on pos [1;1]
    unsigned frame {}; // frame * y-size of the texture = vertical coords
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, AnimationData* animData, TypeTag<AnimationData>);
//inline int LuaImplReplyVal(const LuaState& state, AnimationData&& animData, TypeTag<AnimationData>);

} // namespace Nz

#include "animationdata.inl"

#endif // ANIMATIONDATA_HPP
