// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "factory.hpp"

anax::Entity make_character(anax::World& w, QGraphicsItem* item, int defXg, int defYg,
                                   unsigned defXPos, unsigned defYPos, unsigned maxhp,
                                   const Orientation& o)
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
