// Copyright (C) 2019 Samy Bensaid
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
    Nz::Vector2ui size {};
    Nz::TextureRef texture; // Texture to apply on the sprite
    Nz::Vector2f offset {}; // offset to apply for the character to be on pos [1;1]
    unsigned frame {}; // frame * y-size of the texture = vertical coords
};


#include <Nazara/Lua/LuaState.hpp>
#include <NDK/LuaAPI.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, AnimationData* animData, TypeTag<AnimationData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    animData->size = state.CheckField<Nz::Vector2ui>("size", Nz::Vector2ui {}, index);
    animData->texture = Nz::TextureLibrary::Get(state.CheckField<Nz::String>("texture", ":/game/unknown", index));
    animData->offset = state.CheckField<Nz::Vector2f>("offset", Nz::Vector2f {}, index);
    animData->frame = 0;

    return 1;
}

/*inline int LuaImplReplyVal(const LuaState& state, AnimationData&& animData, TypeTag<AnimationData>)
{
    state.PushTable();
    {
        state.PushField("size", animData.size);
        state.PushField("texture", animData.texture); <-- For this function to work, I'll need to retrieve texture ID (like ":/game/xxx") from texture
        state.PushField("offset", animData.offset);
    }

    return 1;
}*/

} // namespace Nz

#endif // ANIMATIONDATA_HPP
