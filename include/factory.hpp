// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <anax/World.hpp>
#include "components.hpp"
#include "util.hpp"

extern anax::Entity make_character(anax::World& w, const QSize& imgsize, QPixmap tex = {},
                                   unsigned maxframe = 0, int defXg = 0, int defYg = 0, unsigned defXPos = 0,
                                   unsigned defYPos = 0, unsigned maxhp = 100,
                                   const Orientation& o = Orientation::Down);

#endif // FACTORY_HPP
