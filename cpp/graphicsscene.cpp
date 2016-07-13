// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "graphicsscene.h"

GraphicsScene::GraphicsScene(anax::World& world, QObject* parent) : QGraphicsScene(parent),
    m_world(world), m_map(), m_charac(), m_maprenderSys(*this), m_renderSys(*this),
    m_inputSys(), m_AISystem(), m_pather(), m_moveSys()/*, m_posRefresh()*/
{
    m_world.addSystem(m_maprenderSys);
    m_world.refresh();

    m_map = m_world.createEntity();
    m_map.addComponent<Components::Map>();
    m_map.activate();

    m_world.refresh();
    m_maprenderSys.update(m_map.getComponent<Components::Map>());


    m_world.addSystem(m_moveSys);
//    m_world.addSystem(m_posRefresh);

    m_world.addSystem(m_renderSys);
    m_world.refresh();


    QSharedPointer<micropather::MicroPather> pather {
        new micropather::MicroPather{ &m_map.getComponent<Components::Map>() } };
    m_pather = pather;
    m_AISystem.setPather(m_pather.data());
    m_world.addSystem(m_AISystem);
    m_world.refresh();


    m_charac = make_character(m_world, new QGraphicsPixmapItem(QPixmap(":/game/char/villager/idle0")), 5, -35,
                             1, 1, 100, Orientation::Down);

    auto& gfx = m_charac.getComponent<Components::GraphicsItem>();
    gfx.setDefaultPos();
    gfx.show();

    m_charac.activate();
    m_world.refresh();


    m_inputSys.setOnClickMove(&m_charac.getComponent<Components::MoveTo>());
    m_inputSys.setOnClickPos(&m_charac.getComponent<Components::Position>());

    connect(&m_looper, &QTimer::timeout, this, &GraphicsScene::loop);
    m_looper.start(0);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
    //Clicked in the map
    //add this click to the event queue

    auto point = e->scenePos();
    Event ev{MouseClickEvent{e->button(), dToU(point.x()), dToU(point.y())}};

    m_eventQueue.add(ev);
}
