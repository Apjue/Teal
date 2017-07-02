// Copyright (C) 2016 Samy Bensaid
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
            : randomMovement { rd }, movInterval { mov }, nbTiles { nb } {}

        bool randomMovement;
        float movInterval;
        unsigned nbTiles;
    };

    using Elements = std::unordered_map<Element, int>;

    ///
    /// \param size Size of the sprite's image
    /// \param pic Sprite of the character
    /// \param mf Max Frame (used for animation)
    /// \param dg Default graphics position (for it to fit in tile 0,0)
    /// \param dl Default logic position
    /// \param mapPos_ Default Map Position
    /// \param mhp Max HPs (used for fights)
    /// \param o_ Default orientation
    /// \param rdMov_ Used for Random Movement settings
    /// \param blockTile_ Blocks the tile
    /// \param name_ Name of the character
    /// \param desc_ Description of the character
    /// \param atk_ Attack Modifier
    /// \param res_ Resistance Modifier
    /// \param level_ Level of the character
    ///

    CharacterData(const Nz::Vector2ui& size = {},
                  const Nz::SpriteRef& pic = nullptr,
                  unsigned mf = 0u,
                  const Nz::Vector2f& dg = {},
                  const Nz::Vector2ui& dl = {},
                  const Nz::Vector2i& mapPos_ = {},
                  unsigned mhp = 100u,
                  const AnimationComponent::AnimationState& animState_ = AnimationComponent::OnMove,
                  const Orientation& o_ = Orientation::Down,
                  const RandomMovement& rdMov_ = RandomMovement(),
                  bool blockTile_ = false,
                  const Nz::String& name_ = "",
                  const Nz::String& desc_ = "",
                  Elements atk_ = {},
                  Elements res_ = {},
                  unsigned level_ = { 1 })

        : imgsize { size }, sprite { pic }, maxframe { mf }, defG { dg }, defL { dl }, maxhp { mhp }, animState { animState_ },
        o { o_ }, mapPos { mapPos_ }, rdMov(rdMov_), blockTile(blockTile_), name(name_), desc(desc_), atk(atk_), res(res_), level { level_ } {}

    ~CharacterData() = default;

    Nz::Vector2ui imgsize;
    Nz::SpriteRef sprite;
    unsigned maxframe;

    Nz::Vector2f defG;
    Nz::Vector2ui defL;

    unsigned maxhp;
    AnimationComponent::AnimationState animState;
    Orientation o;

    Nz::Vector2i mapPos;
    RandomMovement rdMov;
    bool blockTile;

    Nz::String name;
    Nz::String desc;

    Elements atk;
    Elements res;

    unsigned level;
};

#endif // CHARACTERDATA_HPP
