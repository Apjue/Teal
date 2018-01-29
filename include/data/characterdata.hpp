// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CHARACTERDATA_HPP
#define CHARACTERDATA_HPP

#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Graphics/InstancedRenderable.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <unordered_map>
#include "util/util.hpp"
#include "global.hpp"
#include "data/elementdata.hpp"
#include "components/common/animationcomponent.hpp"

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
            : randomMovement { rd }, movInterval { mov }, range { nb } {}

        bool randomMovement;
        float movInterval;
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
    /// \param dg Default graphics position (for it to fit in tile 0,0)
    /// \param anims List of animations
    /// \param defAnim Default animation
    /// \param o_ Default orientation
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
                  const Nz::Vector2f& dg = {},
                  const AnimationComponent::AnimationList& anims = {},
                  std::size_t defAnim = AnimationComponent::InvalidAnimationID,
                  unsigned mhp = 100u,
                  const Orientation& o_ = Orientation::Down,
                  const RandomMovement& rdMov_ = RandomMovement(),
                  bool blockTile_ = false,
                  const Nz::String& name_ = "",
                  const Nz::String& desc_ = "",
                  Elements atk_ = {},
                  Elements res_ = {},
                  unsigned level_ = 1,
                  const Fight& fight_ = {})

        : codename { name }, sprite { pic }, defG { dg }, animations { anims }, defaultAnimation { defAnim }, maxhp { mhp }, o { o_ }, rdMov(rdMov_), blockTile(blockTile_), name(name_),
          desc(desc_), atk(atk_), res(res_), level { level_ }, fight { fight_ }
    {}

    ~CharacterData() = default;

    Nz::String codename;

    Nz::SpriteRef sprite;
    Nz::Vector2f  defG;

    AnimationComponent::AnimationList animations;
    std::size_t defaultAnimation;

    unsigned maxhp;
    Orientation o;

    RandomMovement rdMov;
    bool blockTile;

    Nz::String name;
    Nz::String desc;

    Elements atk;
    Elements res;

    unsigned level;
    Fight fight;
};

#endif // CHARACTERDATA_HPP
