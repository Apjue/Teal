// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <anax/World.hpp>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QSharedPointer>
#include <QLabel>

#include "components.hpp"
#include "systems.hpp"
#include "util.hpp"
#include "event.hpp"
#include "factory.hpp"
#include "chrono.hpp"
#include "characterinfos.hpp"

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(anax::World& world, QLabel* outputFps = nullptr, QObject* parent = nullptr);
    ~GraphicsScene() = default;

    inline const anax::World& getWorld() const noexcept;
    inline anax::Entity& getPerso() noexcept;
    inline anax::Entity& getMap() noexcept;
    inline void updateMap();

    void setFpsOutput(QLabel* output);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e) override;

private:
    anax::World& m_world;
    anax::Entity m_map;
    anax::Entity m_charac; //main character
    QTimer m_looper{};
    EventQueue m_eventQueue;
    Chrono m_chrono;
    unsigned m_fpsCounter{0};
    unsigned m_fps{};
    QLabel* m_outputFps{}; //We will show fps here

    //Systems
    Systems::MapRenderSystem m_maprenderSys;
    Systems::GraphicsRenderSystem m_renderSys;
    Systems::InputSystem m_inputSys; //Not really a system
    Systems::AISystem m_AISystem;
        std::shared_ptr<micropather::MicroPather> m_pather;
    Systems::MovementSystem m_moveSys;
    Systems::PosRefreshSystem m_posRefresh;
    Systems::AnimationSystem m_animSys;
//    Systems::ItemSystem m_itemSys;
//    Systems::InventorySystem m_invSys;

    void loop()
    {
        m_world.refresh();
        Event e = m_eventQueue.getNext();

        if (e.isValid())
        {
            m_inputSys.notify(e);
//            m_invSys.notify(e);
        }

        auto elapsed = m_chrono.getElapsedTime().asMiliseconds();

        ++m_fpsCounter;

        if (elapsed >= 1000u) //1 second in miliseconds
        {
            m_fps = m_fpsCounter;
            m_fpsCounter = 0;
            writeFpsToOutput();
        }

        m_renderSys.update(elapsed);
        m_AISystem.update(elapsed);
        m_moveSys.update(elapsed);
        m_posRefresh.update(elapsed);
        m_animSys.update(elapsed);

        m_chrono.restart();
    }

    inline void writeFpsToOutput();

    void addSystems();
    void addEntities();
    void initEntities();
    void initSystems();
};

#include "graphicsscene.hpp.inl"

#endif // GRAPHICSSCENE_H
