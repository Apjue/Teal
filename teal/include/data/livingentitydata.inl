// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/LuaAPI.hpp>
#include "util/luaparser.hpp"
#include "util/nzstlcompatibility.hpp"
#include "livingentitydata.hpp"

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, LivingEntityData::Fight* fight, TypeTag<LivingEntityData::Fight>)
{
    if (state.GetType(index) != Nz::LuaType_Table)
        return 1;

    fight->fight = true;
    fight->autoAttack = state.CheckField<bool>("autoAttack", false, index);
    fight->movementPoints = state.CheckField<unsigned>("movementPoints", 3, index);
    fight->actionPoints = state.CheckField<unsigned>("actionPoints", 6, index);

    return 1;
}

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, LivingEntityData* data, TypeTag<LivingEntityData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    data->maxHp = state.CheckField<unsigned>("maxhealth", 100u, index);
    data->orientation = stringToOrientation(state.CheckField<Nz::String>("orientation", "downleft", index));
    data->name = state.CheckField<Nz::String>("name", "Unnamed", index);
    data->description = state.CheckField<Nz::String>("description", "Empty", index);
    data->level = state.CheckField<unsigned>("level", 1u, index);
    data->offset = state.CheckField<Nz::Vector2f>("offset", Nz::Vector2f {}, index);
    data->fight = state.CheckField<LivingEntityData::Fight>("fight", index);
    data->blockTile = state.CheckField<bool>("blocktile", false, index);


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


    if (state.GetField("attack", index) == Nz::LuaType_Table)
        for (Element e {}; e <= Element::Max; ++e)
            data->damageData[e].attack = state.CheckField<int>(elementToString(e), 0, -1);

    state.Pop();

    if (state.GetField("resistance", index) == Nz::LuaType_Table)
        for (Element e {}; e <= Element::Max; ++e)
            data->damageData[e].resistance = state.CheckField<int>(elementToString(e), 0, -1);

    state.Pop();


    Nz::MaterialRef charMat = Nz::Material::New();
    charMat->Configure("Translucent2D");
    charMat->EnableDepthSorting(true);
    charMat->SetDiffuseMap(Nz::TextureLibrary::Get(state.CheckField<Nz::String>("texture", ":/game/unknown", index)));

    auto size = state.CheckField<Nz::Vector2ui>("size", Nz::Vector2ui(charMat->GetDiffuseMap()->GetSize()), index);

    data->sprite = Nz::Sprite::New(charMat);
    data->sprite->SetMaterial(charMat, false);
    data->sprite->SetTextureRect({ 0u, 0u, size.x, size.y });
    data->sprite->SetSize(float(size.x), float(size.y));

    return 1;
}

} // namespace Nz
