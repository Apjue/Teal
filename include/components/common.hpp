// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef COMMON_HPP
#define COMMON_HPP

#include <NDK/World.hpp>
#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>
#include <Nazara/Core/String.hpp>

#include <array>
#include <vector>
#include <tuple>
#include <utility>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <micropather.h>
#include "global.hpp"
#include "util.hpp"

namespace Items
{

struct Item;
struct Equippable;
struct Edible;
struct Resource;

struct AttackBonus;
struct AttackResistance;

}

namespace Components
{

struct RandomMovement : public Ndk::Component<RandomMovement>
{
    RandomMovement() = default;
    ~RandomMovement() = default;

    float movingInterval {};
    unsigned nbTiles {}; // Number of tiles to move each movement

    static Ndk::ComponentIndex componentIndex;
};

///
/// \struct DefaultGraphicsPos
///
/// \brief Used by the factory for the sprite to fit
///        the default logic position
///

struct DefaultGraphicsPos : public Ndk::Component<DefaultGraphicsPos>
{
    DefaultGraphicsPos(float x_, float y_) : x { x_ }, y { y_ } {}

    float x {};
    float y {};

    static Ndk::ComponentIndex componentIndex;
};

struct Name : public Ndk::Component<Name>
{
    Nz::String name {};

    static Ndk::ComponentIndex componentIndex;
};

struct Level : public Ndk::Component<Level>
{
    unsigned level {};

    static Ndk::ComponentIndex componentIndex;
};

///
///
/// \class Inventory
///
/// \brief Contains groups of items
///
/// \note Items are entities which at least have
///       the Item Component
///

class Inventory : public Ndk::Component<Inventory>
{
    using EntityType = Ndk::EntityHandle;
    using EntityCache = std::unordered_set<EntityType>;

public:
    class Group
    {
    public:
        Group() = default;
        Group(const std::string& name_) : name { name_ } {}
        ~Group() = default;

        void add(const EntityType& e)
        {
            if (entities.find(e) != entities.end())
                entities.insert(e);
        }
        void remove(const EntityType& e)
        {
            auto it = entities.find(e);

            if (it != entities.end())
                entities.erase(it);
        }

        std::string name { "undefined" }; // ID of the group
        EntityCache entities; // entities of the group
    };

    Inventory(Ndk::WorldHandle& world) : m_world(world)
    {
        reset();
    }
    ~Inventory() = default;

    void add(const EntityType& e);
    void remove(const EntityType& e);
    bool has(const EntityType& e);

    const EntityCache& getAll();
    inline const Group& group(const std::string& name)
    {
        return m_groups[name];
    }

    static Ndk::ComponentIndex componentIndex;

private:
    Ndk::WorldHandle m_world;
    std::unordered_map<std::string, Group> m_groups;

    ///
    /// \fn reset
    ///
    /// \brief Clears the inventory
    ///        Also used to init the inventory with empty groups
    ///

    void reset();

    ///
    /// \fn assertItem
    ///
    /// \brief Ensures the entity is an item
    ///
    /// \param entity Entity to check
    ///

    void assertItem(const Ndk::EntityHandle& entity) const
    {
        NazaraAssert(entity.IsValid(), "Handle isn't valid !");
        NazaraAssert(entity->IsValid(), "Entity isn't valid !");
        NazaraAssert(entity->HasComponent<Items::Item>(), "Entity isn't an item !");
    }
};

///
/// \struct CDirection
///
/// \brief Contains the Orientation enum
///
/// \todo Change name to Orientation ?
///

struct CDirection : public Ndk::Component<CDirection>
{
    CDirection(const Orientation& o = Orientation::Down) : dir{o} {}
    CDirection(const CDirection&) = default;
    Orientation dir {};

    static Ndk::ComponentIndex componentIndex;
};

struct Animation : public Ndk::Component<Animation>
{
    enum AnimationState
    {
        Undefined,
        Moving
    };

    ///
    /// \fn Animation
    ///
    /// \param s Size of the picture
    /// \param mf Max Frames of the animation
    /// \param df Default Frame of the animation
    ///

    Animation(const Nz::Rectui& s, unsigned mf = 0, AnimationState state = Undefined, unsigned df = 0)
        : frame { df }, size { s }, maxframe { mf }, animationState { state } {}

    unsigned frame {}; // frame * size of the image = vertical coords of the image
    Nz::Rectui size {};
    unsigned maxframe {};
//     bool animated { false };
    AnimationState animationState;

    static Ndk::ComponentIndex componentIndex;
};

struct Position : public Ndk::Component<Position>
{
    Position(unsigned x_ = 0, unsigned y_ = 0) : x{x_}, y{y_} {}

    unsigned x {}; //The actual position
    unsigned y {}; //(absolute)

    //num:
    //X: +32px == +1
    //Y: +16px == +1

    int inX {}; //Position in tile from x/y.
    int inY {}; //difference.

    bool moving { false };

    static Ndk::ComponentIndex componentIndex;
};

struct MoveTo : public Ndk::Component<MoveTo>
{
    int diffX {}; //The position where we want to go
    int diffY {}; //Adds it to the Position's XY to get the tile
    //0 == nowhere

    static Ndk::ComponentIndex componentIndex;
};

struct Path : public Ndk::Component<Path>
{
    std::queue<std::pair<Direction::Dir, bool>> path;
    //To add: path.push(...);
    //To read: path.front(); && path.pop();

    static Ndk::ComponentIndex componentIndex;
};

struct Fight : public Ndk::Component<Fight>
{
    bool isFighting { false };

    static Ndk::ComponentIndex componentIndex;
};

class Life : public Ndk::Component<Life>
{
public:
    Life(unsigned maxhp) : m_hp { maxhp }, m_maxhp { maxhp } {}
    ~Life() = default;

    inline bool alive() const noexcept
    {
        return m_hp == 0;
    }
    inline unsigned HP() const noexcept
    {
        return m_hp;
    }
    inline void setHP(unsigned hp)
    {
        m_hp = hp;
        verifyInfos();
    }

    inline unsigned maxHP() const noexcept
    {
        return m_maxhp;
    }
    inline void setMaxHP(unsigned maxHP)
    {
        m_maxhp = maxHP;
        verifyInfos();
    }

    static Ndk::ComponentIndex componentIndex;

private:
    unsigned m_hp { 100 };
    unsigned m_maxhp { 100 };

    void verifyInfos()
    {
        m_hp = (m_hp > m_maxhp) ? m_maxhp : m_hp;
    }
};

///
/// \class Map
///
/// \brief Map of the game
///        Only one instance of it should exist
///

class Map : public Ndk::Component<Map>, public micropather::Graph
{
public:
    Map() = default;
    Map(const OLDTILEARRAY& _map,
        const OLDTILEARRAY& _obs)
        : map(_map), obs(_obs) {}

    Map(const Map&) = default;
    Map& operator=(const Map&) = default;

    Map(Map&&) = default;
    Map& operator=(Map&&) = default;

    ~Map() = default;

    OLDTILEARRAY map;
    OLDTILEARRAY obs; //0 = can pass, 1 = can't pass but can view through (in fight), 2 = can't pass and can't view through


    //Utility
    static void NodeToXY(void* node, unsigned& x, unsigned& y)
    {
        int index = (int) node;
        y = index / Def::MAPX;
        x = index - y * Def::MAPX;
    }

    static inline void* XYToNode(const unsigned& x, const unsigned& y)
    {
        return (void*) (y * Def::MAPX + x);
    }

    static inline void XYToArray(const unsigned& /*x*/, unsigned& y)
    {
        y /= 2;
    }

    static Ndk::ComponentIndex componentIndex;

private:
    bool passable(unsigned sX, unsigned sY, unsigned eX, unsigned eY);

    //Micropather
    virtual float LeastCostEstimate(void* nodeStart, void* nodeEnd) override;
    virtual void AdjacentCost(void* node, std::vector<micropather::StateCost>* neighbors) override;
    virtual void PrintStateInfo(void* /*node*/) override {}
};

}

#endif // COMMON_HPP
