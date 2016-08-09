// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <anax/World.hpp>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <assert.h>
#include <QPainter>
#include <QDebug>
#include <utility>

#include "basesystems.hpp"
#include "components.hpp"
#include "global.hpp"
#include "event.hpp"
#include "util.hpp"
#include "chrono.hpp"

namespace Systems
{

///
/// \brief The InventorySystem class
///
/// If notified by an event which says an item has been destroyed,
/// It removes the entity item from all inventories
///

class InventorySystem : public anax::System<anax::Requires<Components::Inventory>>, public BaseSystem
{
public:
    InventorySystem(anax::World& world) : m_world(world) {}
    ~InventorySystem() = default;

    virtual void notify(const Event& e) override
    {
        if (e == Event::ItemDestroyedEvent)
            handleDestroyed(e.itemDestroyedEvent.id);
    }

private:
    anax::World& m_world;

    ///
    /// \todo Use deactivate function of inventories
    ///

    void handleDestroyed(const anax::Entity::Id& id)
    {
        auto entities = getEntities();

        for (auto& e: entities)
        {
            e.getComponent<Components::Inventory>().remove(id);
        }
    }
};

///
/// \brief The ItemSystem class
///
/// Check when an Entity Item is killed,
/// and add an event to the event queue when that occurs
///
/// \note This system is passive: No need to update
///

class ItemSystem : public anax::System<anax::Requires<Items::Item>>, public BaseSystem
{
public:
    ItemSystem(EventQueue& eq) : m_eventqueue{eq} {}
    ~ItemSystem() = default;

    virtual void notify(const Event&) override {}

private:
    EventQueue& m_eventqueue;

    void onEntityRemoved(anax::Entity& item)
    {
        m_eventqueue.add( Event{ Events::ItemDestroyed {item.getId()} } );
    }
};

///
/// \brief The AnimationSystem class
///
/// Animates Graphics Items, as long as
/// their type is QGraphicsPixmapItem
///

class AnimationSystem : public anax::System<anax::Requires<
        Components::Animation, Components::GraphicsItem, Components::Position>>, public BaseSystem
{
public:
    AnimationSystem() = default;
    ~AnimationSystem() = default;

    virtual void notify(const Event&) override {}

    ///
    /// \todo Remove the "updateFps" and add an fps counter in the component
    /// .......
    ///

    void update(Miliseconds elapsed);

private:
    Miliseconds m_fpsCount{};

    bool updateFps(Miliseconds fpsToAdd);
};

///
/// \brief The PosRefreshSystem class
///
/// Refresh the graphics position of items
/// using their logic position
/// [WORKAROUND 2]
///

class PosRefreshSystem : public anax::System<anax::Requires<
      Components::Position, Components::GraphicsItem>>, public BaseSystem
{
public:
    PosRefreshSystem() = default;
    ~PosRefreshSystem() = default;

    virtual void notify(const Event&) override {}
    void update(Miliseconds);
};

///
/// \brief The InputSystem class
///
/// Gets events with notify()
/// and change Components values
///

class InputSystem : public BaseSystem
{
public:
    InputSystem() = default;
    InputSystem(const std::pair<Components::MoveTo*, Components::Position*>& onClick)
        : m_onClickMove(onClick.first), m_onClickPos(onClick.second) {}
    ~InputSystem() = default;

    virtual void notify(const Event& e) override;

    ///
    /// \todo Add the z axis ?
    ///

    AbsTile getTileFromClick(const Events::MouseClick& e) const;

    inline void setOnClickMove(Components::MoveTo* nc)
    {
        m_onClickMove = nc;
    }
    inline void setOnClickPos(Components::Position* nc)
    {
        m_onClickPos = nc;
    }

private:
    QImage m_scheme{":/game/scheme"};

    Components::MoveTo* m_onClickMove{};
    Components::Position* m_onClickPos{};

    void handleEvent(const Events::MouseClick& e);
};

///
/// \brief The AISystem class
///
/// Compute a path with the Position and MoveTo component
/// And write it to the Path component.
/// (uses micropather)
///

class AISystem : public anax::System<anax::Requires<Components::Path,
        Components::Position, Components::MoveTo>>, public BaseSystem
{
public:
    AISystem() = default;
    ~AISystem() = default;
    virtual void notify(const Event&) override {}

    inline void reset()
    {
        assert(m_pather && "Pather is null !");
        m_pather->Reset();
    }
    void setPather(std::shared_ptr<micropather::MicroPather> pather)
    {
        m_pather = pather;
    }
    void update(Miliseconds);

private:
    std::shared_ptr<micropather::MicroPather> m_pather{};
};

///
/// \brief The MovementSystem class
///
/// Moves any entity which has a path component
/// and a position component.
/// If the path is empty, no move.
/// The move isn't done with one call to update()
///

class MovementSystem : public anax::System<anax::Requires<Components::Path, Components::Position,
        Components::CDirection>>, public BaseSystem
{
public:
    MovementSystem() = default;
    ~MovementSystem() = default;
    virtual void notify(const Event&) override {}

    void update(Miliseconds elapsed);

private:
    Miliseconds m_fpsCount{};

    bool updateFps(Miliseconds fpsToAdd);
};

///
/// \brief The MapRenderSystem class
///
/// Draws a map. What did you expect ?
///

class MapRenderSystem : public anax::System<anax::Requires<Components::Map>>, public BaseRenderSystem
{
public:
    MapRenderSystem() = delete;
    MapRenderSystem(QGraphicsScene& scene, Components::Map* mapTab = nullptr)
        : m_mapTab(mapTab)
    {
        setScene(scene);
        m_map = getScene().addPixmap(QPixmap());
    }
    ~MapRenderSystem() = default;

    ///
    /// \brief update
    ///
    /// This function updates the map using m_mapTab
    /// It must be called only when the array changed.
    ///
    /// \todo Render in Fight Mode & render map border
    ///

    void update();
    void update(Components::Map& tab)
    {
        m_mapTab = &tab;
        update();
    }

private:
    QGraphicsPixmapItem* m_map{};
    Components::Map* m_mapTab{};

    void renderMap(const Components::Map& comp, QPainter& painter);
};

///
/// \brief The GraphicsRenderSystem class
///
/// Renders graphics items by calling render().
/// It only shows or hide graphics items.
///

class GraphicsRenderSystem : public anax::System<anax::Requires<Components::GraphicsItem>>, public BaseRenderSystem
{

public:
    GraphicsRenderSystem() = default;
    GraphicsRenderSystem(QGraphicsScene& scene) noexcept
    {
        setScene(scene);
    }
    ~GraphicsRenderSystem() = default;

    ///
    /// \brief update
    ///
    /// Must be called each frame to update the screen
    /// To stop rendering an entity: deactivate GraphicsItem.
    /// You can also deactivate the entity AFTER deactivating the GraphicsItem
    /// and calling this function, but it's not needed
    ///
    /// When adding an entity, do not forget to activate the GraphicsItem, or else
    /// the GraphicsItem won't be visible.
    ///

    void update(Miliseconds);
};

}

#endif // SYSTEMS_HPP
