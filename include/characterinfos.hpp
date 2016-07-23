// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CHARACTERINFOS_HPP
#define CHARACTERINFOS_HPP

#include <QSize>
#include <QPixmap>

#include "util.hpp"
#include "global.hpp"
#include "vector2.hpp"

///
/// \brief The CharacterInfos struct
///
/// This class contains all required attributes of a character
///

struct CharacterInfos
{
    CharacterInfos(const QSize& s, const QPixmap& p, unsigned mf = 0,
                   const Vector2i dg = {}, const Vector2u dl = {},
                   unsigned mhp = 100, const Orientation& o_ = {Orientation::Down})
        : imgsize{s}, tex{p}, maxframe{mf}, defG{dg}, defL{dl}, maxhp{mhp}, o{o_} {}
    ~CharacterInfos() = default;

    QSize imgsize;
    QPixmap tex;
    unsigned maxframe{};

    Vector2i defG{0, 0}; //default graphics pos
    Vector2u defL{0u, 0u}; //default logic pos

    unsigned maxhp{100};
    Orientation o {Orientation::Down};
};

#endif // CHARACTERINFOS_HPP
