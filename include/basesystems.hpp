// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef BASESYSTEMS_HPP
#define BASESYSTEMS_HPP

#include "event.hpp"
#include <QGraphicsScene>

namespace Systems
{

///
/// \brief The BaseSystem class
///
/// Base class of all systems
///

class BaseSystem
{
public:
    BaseSystem() = default;
    ~BaseSystem() = default;

    virtual void notify(const Event&) = 0;
};

///
/// \brief The BaseRenderSystem class
///
/// Base class of all render systems
///

class BaseRenderSystem : public BaseSystem
{

public:
    BaseRenderSystem() = default;
    ~BaseRenderSystem() = default;
    virtual void notify(const Event&) override {}

    void setScene(QGraphicsScene& scene) noexcept
    {
        m_scene = &scene;
    }
    inline QGraphicsScene& getScene() const noexcept
    {
        assert(isValid() && "Render system is not valid");
        return *m_scene;
    }
    virtual inline bool isValid() const
    {
        return (m_scene);
    }

private:
    QGraphicsScene* m_scene{};
};

}

#endif // BASESYSTEMS_HPP
