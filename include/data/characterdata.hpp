// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CHARACTERDATA_HPP
#define CHARACTERDATA_HPP

#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Graphics/InstancedRenderable.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <unordered_map>
#include "components/common/animationcomponent.hpp"
#include "data/elementdata.hpp"
#include "cache/doublestore.hpp"
#include "util/util.hpp"
#include "global.hpp"

///
/// \struct CharacterData
///
/// \brief Contains all required attributes of a character
///        It is used by a character factory
///

struct CharacterData
{
    struct RandomMovement
    {
        RandomMovement(bool rd = false, float mov = 7.5f, unsigned nb = 1u)
            : randomMovement { rd }, movementInterval { mov }, range { nb } {}

        bool randomMovement;
        float movementInterval;
        unsigned range;
    };

    struct Fight
    {
        Fight(bool f = false, bool autoAtk = false, unsigned mov = 3, unsigned action = 6)
            : fight { f }, autoAttack { autoAtk }, movementPoints { mov }, actionPoints { action } {}

        bool fight;
        bool autoAttack;
        unsigned movementPoints;
        unsigned actionPoints;
    };

    using Elements = std::unordered_map<Element, int>;

    ///
    /// \param name Codename of the character (for illegal purposes, i.e. cloning)
    /// \param size Size of the sprite's image
    /// \param pic Sprite of the character
    /// \param offset Default graphics position (for it to fit in tile 0,0)
    /// \param anims List of animations
    /// \param defAnim Default animation
    /// \param orientation_ Default orientation
    /// \param rdMov_ Used for Random Movement settings
    /// \param blockTile_ Blocks the tile
    /// \param name_ Name of the character
    /// \param desc_ Description of the character
    /// \param atk_ Attack Modifier
    /// \param res_ Resistance Modifier
    /// \param level_ Level of the character
    /// \param fight_ Used for fight settings
    ///

    CharacterData(const Nz::String& name,
                  const Nz::SpriteRef& pic = nullptr,
                  const Nz::Vector2f& offset_ = {},
                  const AnimationComponent::AnimationList& anims = {},
                  std::size_t defAnim = AnimationComponent::InvalidAnimationID,
                  unsigned mhp = 100u,
                  const Orientation& orientation_ = Orientation::Down,
                  const RandomMovement& rdMov_ = RandomMovement(),
                  bool blockTile_ = false,
                  const Nz::String& name_ = "",
                  const Nz::String& desc_ = "",
                  Elements atk_ = {},
                  Elements res_ = {},
                  unsigned level_ = 1,
                  const Fight& fight_ = {})

        : codename { name }, sprite { pic }, offset { offset_ }, animations { anims }, defaultAnimation { defAnim }, maxHp { mhp }, orientation { orientation_ }, randomMovement(rdMov_), blockTile(blockTile_), name(name_),
          description(desc_), attack(atk_), resistance(res_), level { level_ }, fight { fight_ }
    {}

    CharacterData() = default;
    ~CharacterData() = default;


    Nz::String codename;

    Nz::SpriteRef sprite;
    Nz::Vector2f  offset;

    AnimationComponent::AnimationList animations;
    std::size_t defaultAnimation;

    unsigned maxHp;
    Orientation orientation;

    RandomMovement randomMovement;
    bool blockTile;

    Nz::String name;
    Nz::String description;

    Elements attack;
    Elements resistance;

    unsigned level;
    Fight fight;
};

#include <Nazara/Lua/LuaState.hpp>
#include <NDK/LuaAPI.hpp>
#include "util/luaparser.hpp"

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

    {
        data->defaultAnimation = AnimationComponent::InvalidAnimationID;

        for (auto& animVariant : animArgs.vars)
        {
            if (!animVariant.is<Nz::String>())
            {
                NazaraError("Animation codename: String expected");
                continue;
            }

            Nz::String& animName = animVariant.get<Nz::String>();

            if (!DoubleStores<AnimationData>::getInstance()->hasItem(animName))
            {
                NazaraError("Animation codename not found: " + animName);
                continue;
            }

            AnimationData anim = DoubleStores<AnimationData>::getInstance()->getItem(animName); // Yup, copy ctor
            data->animations.emplace_back(std::move(anim));
        }

        if (!data->animations.empty())
            data->defaultAnimation = state.CheckField<std::size_t>("default", 0, -1);
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

#endif // CHARACTERDATA_HPP
