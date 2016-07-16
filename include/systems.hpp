// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

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
/// \brief The AnimationSystem class
///
/// Animates Graphics Items, as long as
/// their type is QGraphicsPixmapItem
///

class AnimationSystem : public anax::System<anax::Requires<
        Components::Animation, Components::GraphicsItem>>, public BaseSystem
{
public:
    AnimationSystem() = default;
    ~AnimationSystem() = default;

    virtual void notify(const Event&) override {}
    void update(Miliseconds);
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
    AbsTile getTileFromClick(const MouseClickEvent& e) const;

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

    void handleEvent(const MouseClickEvent& e);
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
    void setPather(micropather::MicroPather* pather)
    {
        m_pather = pather;
    }
    void update(Miliseconds);

private:
    micropather::MicroPather* m_pather{};
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
/// Does not animate items. Only render them.
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
