// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <anax/World.hpp>
#include "components.hpp"
#include "util.hpp"

inline anax::Entity make_character(anax::World& w, QGraphicsItem* item = nullptr, int defXg = 0, int defYg = 0,
                                   unsigned defXPos = 0, unsigned defYPos = 0, unsigned maxhp = 100,
                                   const Orientation& o = Orientation::Down)
{
    anax::Entity e = w.createEntity();

    e.addComponent<Components::GraphicsItem>(item, defXg, defYg);

    e.addComponent<Components::Life>(maxhp);
    e.addComponent<Components::Fight>();

    e.addComponent<Components::CDirection>(o);
    e.addComponent<Components::Position>(defXPos, defYPos);
    e.addComponent<Components::MoveTo>();
    e.addComponent<Components::Path>();

    e.addComponent<Components::Animation>
      (e.getComponent<Components::CDirection>().dir);

    w.refresh();
    return e;
}

#endif // FACTORY_HPP
