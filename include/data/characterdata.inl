// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_CHARACTERDATA_INL
#define TEAL_CHARACTERDATA_INL

#include <NDK/LuaAPI.hpp>
#include "util/luaparser.hpp"
#include "characterdata.hpp"

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, CharacterData::RandomMovement* random, TypeTag<CharacterData::RandomMovement>)
{
    if (state.GetType(index) != Nz::LuaType_Table)
        return 1;

    random->randomMovement = true;
    random->movementInterval = state.CheckField<float>("movement_interval", index);
    random->range = state.CheckField<unsigned>("range", index);

    return 1;
}

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, CharacterData::Fight* fight, TypeTag<CharacterData::Fight>)
{
    if (state.GetType(index) != Nz::LuaType_Table)
        return 1;

    fight->fight = true;
    fight->autoAttack = state.CheckField<bool>("autoAttack", false, index);
    fight->movementPoints = state.CheckField<unsigned>("movementPoints", 3, index);
    fight->actionPoints = state.CheckField<unsigned>("actionPoints", 6, index);

    return 1;
}

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, CharacterData* data, TypeTag<CharacterData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    data->maxHp = state.CheckField<unsigned>("maxhealth", 100u, index);
    data->orientation = stringToOrientation(state.CheckField<Nz::String>("orientation", "downleft", index));
    data->blockTile = state.CheckField<bool>("blocktile", false, index);
    data->name = state.CheckField<Nz::String>("name", "Unnamed", index);
    data->description = state.CheckField<Nz::String>("description", "Empty", index);
    data->level = state.CheckField<unsigned>("level", 1u, index);
    data->offset = state.CheckField<Nz::Vector2f>("offset", Nz::Vector2f {}, index);


    LuaArguments animArgs;

    if (state.GetField("animations") == Nz::LuaType_Table)
        parseLua(state, animArgs);

    for (auto& animTable : animArgs.tables)
    {
        auto& animPair = animTable->vars;

        // Check everything is ok
        {
            if (animPair.size() != 2)
            {
                NazaraError("Animation: Pair expected (i.e. { \"type\", \"animation_codename\" }");
                continue;
            }

            if (!animPair[0].is<Nz::String>() || !animPair[1].is<Nz::String>())
            {
                NazaraError("Animation: Pair with strings expected  (i.e. { \"type\", \"animation_codename\" }");
                continue;
            }
        }

        Nz::String& animType = animPair[0].get<Nz::String>();
        Nz::String& animName = animPair[1].get<Nz::String>();

        if (!DoubleStores<AnimationData>::getInstance()->hasItem(animName))
        {
            NazaraError("Animation codename not found: " + animName);
            continue;
        }

        AnimationData anim = DoubleStores<AnimationData>::getInstance()->getItem(animName); // Important: use copy ctor
        data->animations[AnimationComponent::stringToAnimationType(animType)] = std::move(anim);
    }

    state.Pop();

    data->randomMovement = state.CheckField<CharacterData::RandomMovement>("random_movement", index);
    data->attack = state.CheckField<CharacterData::Elements>("attack", index);
    data->resistance = state.CheckField<CharacterData::Elements>("resistance", index);
    data->fight = state.CheckField<CharacterData::Fight>("fight", index);


    Nz::MaterialRef charMat = Nz::Material::New();
    charMat->Configure("Translucent2D");
    charMat->SetDiffuseMap(Nz::TextureLibrary::Get(state.CheckField<Nz::String>("texture", ":/game/unknown", index)));

    auto size = state.CheckField<Nz::Vector2ui>("size", Nz::Vector2ui {}, index);

    data->sprite = Nz::Sprite::New(charMat);
    data->sprite->SetMaterial(charMat, false);
    data->sprite->SetTextureRect({ 0u, 0u, size.x, size.y });
    data->sprite->SetSize(float(size.x), float(size.y));

    return 1;
}

} // namespace Nz

#endif // TEAL_CHARACTERDATA_INL
