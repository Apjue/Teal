// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <anax/World.hpp>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QSharedPointer>

#include "components.hpp"
#include "systems.hpp"
#include "util.hpp"
#include "event.hpp"
#include "factory.hpp"
#include "chrono.hpp"

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(anax::World& world, QObject* parent = nullptr);
    ~GraphicsScene() = default;

    inline const anax::World& getWorld() const noexcept
    {
        return m_world;
    }
    inline anax::Entity& getPerso() noexcept
    {
        return m_charac;
    }
    inline anax::Entity& getMap() noexcept
    {
        return m_map;
    }
    inline void updateMap()
    {
        m_maprenderSys.update();
    }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e) override;

private:
    anax::World& m_world;
    anax::Entity m_map;
    anax::Entity m_charac; //main character
    QTimer m_looper{};
    EventQueue m_eventQueue;
    Chrono m_chrono;

    //Systems
    Systems::MapRenderSystem m_maprenderSys;
    Systems::GraphicsRenderSystem m_renderSys;
    Systems::InputSystem m_inputSys;
    Systems::AISystem m_AISystem;
        QSharedPointer<micropather::MicroPather> m_pather;
    Systems::MovementSystem m_moveSys;
    Systems::PosRefreshSystem m_posRefresh;
    Systems::AnimationSystem m_animSys;

    void loop()
    {
        m_world.refresh();
        Event e = m_eventQueue.getNext();

        if (e.isValid())
        {
            m_inputSys.notify(e);
        }

        auto elapsed = m_chrono.getElapsedTime().asMiliseconds();

        m_renderSys.update(elapsed);
        m_AISystem.update(elapsed);
        m_moveSys.update(elapsed);
        m_posRefresh.update(elapsed);
        m_animSys.update(elapsed);

        m_chrono.restart();
    }
};

#endif // GRAPHICSSCENE_H
