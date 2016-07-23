// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "factory.hpp"

anax::Entity make_character(anax::World& w, const CharacterInfos& c)
{
    anax::Entity e = w.createEntity();

    QGraphicsPixmapItem* pix = new QGraphicsPixmapItem{};

    setTextureRect(*pix, c.tex, QRect{0, 0, c.imgsize.width(), c.imgsize.height()} );
    e.addComponent<Components::GraphicsItem>(pix, c.defG);


    e.addComponent<Components::Life>(c.maxhp);
    e.addComponent<Components::Fight>();

    e.addComponent<Components::CDirection>(c.o);
    e.addComponent<Components::Position>(c.defL.first(), c.defL.second());
    e.addComponent<Components::MoveTo>();
    e.addComponent<Components::Path>();

    e.addComponent<Components::Animation>
      (e.getComponent<Components::CDirection>().dir, c.imgsize, c.tex, c.maxframe);

    w.refresh();
    return e;
}
