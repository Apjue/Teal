// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsSimpleTextItem>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsObject>
#include <QGraphicsItem>

#include <QGraphicsScene>
#include <anax/Component.hpp>
#include <anax/Entity.hpp>
#include <array>
#include <vector>
#include <tuple>
#include <utility>
#include <queue>
#include <micropather.h>
#include "global.hpp"
#include "util.hpp"

namespace Components
{

struct CDirection : public anax::Component
{
    CDirection(const Orientation& o = Orientation::Down) : dir{o} {}
    CDirection(const CDirection&) = default;
    Orientation dir{};
};

struct Animation : public anax::Component
{
    Animation(Orientation& d) : dir{d} {}
    Orientation& dir; // dir * size of img = img (x)
    unsigned frame; // frame * size of img = img (y)
};

struct Position : public anax::Component
{
    Position(unsigned x_ = 0, unsigned y_ = 0) : x{x_}, y{y_} {}

    unsigned x{}; //The actual position
    unsigned y{}; //(absolute)

    //num:
    //X: +32px == +1
    //Y: +16px == +1

    int inX{0}; //Position in tile from x/y.
    int inY{0}; //difference.
};

struct MoveTo : public anax::Component
{
    int diffX{}; //The position we want to go
    int diffY{}; //Adds it to the Position's XY to get the tile
    //0 == nowhere
};

struct Path : public anax::Component
{
    std::queue<std::pair<Direction::Dir, bool>> path;
    //To add: path.push(...);
    //To read: path.front(); && path.pop();
};

struct Fight : public anax::Component
{
    bool fight{false};
};

class Life : public anax::Component
{
public:
    Life(unsigned maxhp) : m_hp{maxhp}, m_maxhp{maxhp} {}
    ~Life() = default;

    bool alive() const noexcept
    {
        return m_alive;
    }
    unsigned HP() const noexcept
    {
        return m_hp;
    }
    void setHP(unsigned hp)
    {
        m_hp = hp;
        verifyInfos();
    }

    unsigned maxHP() const noexcept
    {
        return m_maxhp;
    }
    void setMaxHP(unsigned newHP)
    {
        m_maxhp = newHP;
        verifyInfos();
    }


private:
    unsigned m_hp{100};
    unsigned m_maxhp{100};
    bool m_alive{true};

    void verifyInfos()
    {
        m_hp = (m_hp > m_maxhp) ? m_maxhp : m_hp;
        m_alive = (m_hp == 0);
    }
};

///
/// \brief The Map class
///
/// Map of the game.
/// Must be rendered first.
/// Only one instance of it.
///

class Map : public anax::Component, public micropather::Graph
{
    Map(Map&&) = delete;
    Map(const Map&) = delete;

    Map& operator=(const Map&) = delete;
    Map& operator=(Map&&) = delete;

public:
    Map() = default;
    Map(const Def::TILEARRAY& _map,
        const Def::TILEARRAY& _obs)
        : map(_map), obs(_obs) {}
    ~Map() = default;

    Def::TILEARRAY map;
    Def::TILEARRAY obs;
    //0 = can pass, 1 = can't pass but can view through (in fight), 2 = can't pass and can't view through


    //Utility
    static void NodeToXY(void* node, unsigned& x, unsigned& y)
    {
        int index = (int) node; //Yep, C-style cast.
        y = index / Def::MAPX;
        x = index - y * Def::MAPX;
    }
    static inline void* XYToNode(const unsigned& x, const unsigned& y)
    {
        return (void*) ( y*Def::MAPX + x ); //C-Style cast... again.
    }
    static inline void XYToArray(const unsigned& /*x*/, unsigned& y)
    {
        y /= 2;
    }

private:
    bool passable(unsigned sX, unsigned sY, unsigned eX, unsigned eY)
    {
        //Step 1.
        {
            if ((sX == eX-2 && sY == eY)
                    || (sX == eX+2 && sY == eY)
                    || (sX == eX && sY == eY-2)
                    || (sX == eX && sY == eY+2)
                    //diagonales
                    || (sX == eX+1 && sY == eY+1)
                    || (sX == eX-1 && sY == eY-1)
                    || (sX == eX+1 && sY == eY-1)
                    || (sX == eX-1 && sY == eY+1))
                ; //ok, continue
            else
                return false;
        }

        //Step 2.
        {
            XYToArray(eX, eY);
            if (eX > Def::MAPX || eY > Def::MAPY)
                return false;

            unsigned tile {eX+eY*Def::MAPX};

            unsigned tileNumber = obs[tile];
            return (tileNumber == 0);
        }
    }

    //Micropather
    virtual float LeastCostEstimate( void* nodeStart, void* nodeEnd ) override
    {
        unsigned sX{}, sY{};
        NodeToXY(nodeStart, sX, sY);

        unsigned eX{}, eY{};
        NodeToXY(nodeEnd, eX, eY);


        unsigned rX{distance(sX, eX)}, rY{distance(sY, eY)};
        unsigned const estimated{rX+rY};

        return static_cast<float>(estimated);
    }
    virtual void AdjacentCost( void* node, std::vector< micropather::StateCost > *neighbors ) override
    {
        static constexpr std::array<int, 8> dx     { 0,   2,   0 , -2,   1,   -1,    1,   -1    };
        static constexpr std::array<int, 8> dy     { 2,   0,  -2,   0,   1,   -1,   -1,    1    };
        static constexpr std::array<float, 8> cost { 2.f, 2.f, 2.f, 2.f, 1.5f, 1.5f, 1.5f, 1.5f };

        unsigned x{}, y{};
        NodeToXY( node, x, y );

        for(unsigned i{}; i < cost.size(); ++i)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (passable(x, y, newX, newY))
            {
                micropather::StateCost nodeCost = { XYToNode( newX, newY ), cost[i] };
                neighbors->push_back( nodeCost );
            }
        }
    }
    virtual void PrintStateInfo( void* /*node*/ ) override
    {
        ;
    }
};

///
/// \brief The GraphicsItem class
///
/// Base class for all Qt graphics Item with anax.
///

class GraphicsItem : public anax::Component
{
    GraphicsItem() = delete;
    GraphicsItem(GraphicsItem&&) = delete;
    GraphicsItem(const GraphicsItem&) = delete;

    GraphicsItem& operator=(const GraphicsItem&) = delete;
    GraphicsItem& operator=(GraphicsItem&&) = delete;

public:
    GraphicsItem(QGraphicsItem* p_item, int defPosX = 0, int defPosY = 0)
        : m_item(p_item), m_defPosX{defPosX}, m_defPosY{defPosY} { item()->setZValue(1); }

    ~GraphicsItem() {}
    std::pair<int, int> defaultPos() const
    {
        return std::make_pair(m_defPosX, m_defPosY);
    }
    void setDefaultPos() //should go to 0,0 logic position
    {
        m_item->setPos(m_defPosX, m_defPosY);
    }
    bool operator==(const GraphicsItem& other) const
    {
        return (m_item == other.m_item)
            && (m_visible == other.m_visible);
    }

    inline void hide()
    {
        m_visible = false;
    }
    inline void show()
    {
        m_visible = true;
    }
    inline bool isVisible()
    {
        return m_visible;
    }
    inline QGraphicsItem* item() const
    {
        return m_item.get();
    }

    QGraphicsPixmapItem* pixmap() const
    {
        return dynamic_cast<QGraphicsPixmapItem*>(item());
    }
    QGraphicsLineItem* line() const
    {
        return dynamic_cast<QGraphicsLineItem*>(item());
    }

    QAbstractGraphicsShapeItem* shape() const
    {
        return dynamic_cast<QAbstractGraphicsShapeItem*>(item());
    }
    QGraphicsRectItem* rect() const
    {
        return dynamic_cast<QGraphicsRectItem*>(shape());
    }
    QGraphicsPolygonItem* polygon() const
    {
        return dynamic_cast<QGraphicsPolygonItem*>(shape());
    }

    QGraphicsObject* object() const
    {
        return dynamic_cast<QGraphicsObject*>(item());
    }
    QGraphicsTextItem* text() const
    {
        return dynamic_cast<QGraphicsTextItem*>(object());
    }
    QGraphicsSimpleTextItem* sText() const
    {
        return dynamic_cast<QGraphicsSimpleTextItem*>(shape());
    }
    QGraphicsWidget* widget() const
    {
        return dynamic_cast<QGraphicsWidget*>(object());
    }
    QGraphicsProxyWidget* proxy() const
    {
        return dynamic_cast<QGraphicsProxyWidget*>(widget());
    }

private:
    non_nullptr<QGraphicsItem> m_item;
    bool m_visible{false};
    int m_defPosX{};
    int m_defPosY{};
};

}

#endif // COMPONENTS_HPP
