// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CHARACTERDATA_HPP
#define CHARACTERDATA_HPP

#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Graphics/InstancedRenderable.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <unordered_map>
#include "components/characters/animationcomponent.hpp"
#include "components/shared/damagemodifiercomponent.hpp"
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

    ///
    /// \param name Codename of the character (for illegal purposes, i.e. cloning)
    /// \param size Size of the sprite's image
    /// \param pic Sprite of the character
    /// \param offset Default graphics position (for it to fit in tile 0,0)
    /// \param anims List of animations
    /// \param orientation_ Default orientation
    /// \param rdMov_ Used for Random Movement settings
    /// \param blockTile_ Blocks the tile
    /// \param name_ Name of the character
    /// \param desc_ Description of the character
    /// \param damageData_ Damage Modifier
    /// \param level_ Level of the character
    /// \param fight_ Used for fight settings
    ///

    CharacterData(const Nz::String& name,
                  const Nz::SpriteRef& pic = nullptr,
                  const Nz::Vector2f& offset_ = {},
                  const AnimationComponent::AnimationList& anims = {},
                  unsigned mhp = 100u,
                  const Orientation& orientation_ = Orientation::Down,
                  const RandomMovement& rdMov_ = RandomMovement(),
                  bool blockTile_ = false,
                  const Nz::String& name_ = "",
                  const Nz::String& desc_ = "",
                  DamageModifierList damageData_ = {},
                  unsigned level_ = 1,
                  const Fight& fight_ = {})

        : codename { name }, sprite { pic }, offset { offset_ }, animations { anims }, maxHp { mhp }, orientation { orientation_ }, randomMovement(rdMov_), blockTile(blockTile_),
        name(name_), description(desc_), damageData(damageData_), level { level_ }, fight { fight_ }
    {}

    CharacterData() = default;
    ~CharacterData() = default;


    Nz::String codename;

    Nz::SpriteRef sprite;
    Nz::Vector2f  offset;
    AnimationComponent::AnimationList animations;

    unsigned maxHp;
    Orientation orientation;

    RandomMovement randomMovement;
    bool blockTile;

    Nz::String name;
    Nz::String description;

    DamageModifierList damageData;

    unsigned level;
    Fight fight;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, CharacterData::RandomMovement* random, TypeTag<CharacterData::RandomMovement>);
inline unsigned int LuaImplQueryArg(const LuaState& state, int index, CharacterData::Fight* fight, TypeTag<CharacterData::Fight>);
inline unsigned int LuaImplQueryArg(const LuaState& state, int index, CharacterData* data, TypeTag<CharacterData>);

} // namespace Nz

#include "characterdata.inl"

#endif // CHARACTERDATA_HPP
