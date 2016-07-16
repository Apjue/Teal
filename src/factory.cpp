// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "factory.hpp"

anax::Entity make_character(anax::World& w, const QSize& imgsize, QPixmap tex, unsigned maxframe,
                            int defXg, int defYg, unsigned defXPos, unsigned defYPos, unsigned maxhp,
                            const Orientation& o)
{
    anax::Entity e = w.createEntity();

    QGraphicsPixmapItem* pix{};

    try
    {
        pix = new QGraphicsPixmapItem{};
    }
    catch(const std::exception&)
    {
        throw;
    }

    setTextureRect(*pix, tex, QRect{0, 0, imgsize.width(), imgsize.height()} );
    e.addComponent<Components::GraphicsItem>(pix, defXg, defYg);


    e.addComponent<Components::Life>(maxhp);
    e.addComponent<Components::Fight>();

    e.addComponent<Components::CDirection>(o);
    e.addComponent<Components::Position>(defXPos, defYPos);
    e.addComponent<Components::MoveTo>();
    e.addComponent<Components::Path>();

    e.addComponent<Components::Animation>
      (e.getComponent<Components::CDirection>().dir, imgsize, tex, maxframe);

    w.refresh();
    return e;
}
