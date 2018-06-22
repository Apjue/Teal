// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_ANIMATIONDATA_INL
#define TEAL_ANIMATIONDATA_INL

#include "animationdata.hpp"

inline AnimationData::AnimationType AnimationData::stringToAnimationType(Nz::String string)
{
    string = string.ToLower();

    if (string == "walk")
        return Walk;

    if (string == "run")
        return Run;

    if (string == "fight")
        return Fight;

    if (string == "emote")
        return Emote;

    throw std::runtime_error { "Undefined animation type!" };
}

inline Nz::String AnimationData::animationTypeToString(AnimationType animType)
{
    switch (animType)
    {
        case AnimationData::Walk:
            return "walk";

        case AnimationData::Run:
            return "run";

        case AnimationData::Fight:
            return "fight";

        case AnimationData::Emote:
            return "emote";
    }

    throw std::runtime_error { "Undefined animation type!" };
}

#include <Nazara/Lua/LuaState.hpp>
#include <NDK/LuaAPI.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, AnimationData* animData, TypeTag<AnimationData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    animData->type = AnimationData::stringToAnimationType(state.CheckField<Nz::String>("type", index));
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
        state.PushField("type", AnimationData::animationTypeToString(animData.type));
        state.PushField("size", animData.size);
        state.PushField("texture", animData.texture); <-- For this function to work, I'll need to retrieve texture ID (like ":/game/xxx") from texture
        state.PushField("offset", animData.offset);
    }

    return 1;
}*/

} // namespace Nz

#endif // TEAL_ANIMATIONDATA_INL
