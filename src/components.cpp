// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components.hpp"

void Components::Inventory::add(anax::Entity::Id& id)
{
    auto entity = m_world.getEntity(id.value());
    assert( entity.hasComponent<Items::Item>() );
    m_groups["all"].entities.insert(id.value());

    //Add to basic groups
    if (entity.hasComponent<Items::Edible>())
        m_groups["edible"].entities.insert(id.value());

    if (entity.hasComponent<Items::Equippable>())
        m_groups["equippable"].entities.insert(id.value());

    if (entity.hasComponent<Items::Resource>())
        m_groups["resource"].entities.insert(id.value());

    //Add to automatic groups
    if (entity.hasComponent<Items::AttackBonus>())
        m_groups["attack"].entities.insert(id.value());

    if (entity.hasComponent<Items::AttackResistance>())
        m_groups["resistance"].entities.insert(id.value());

}

void Components::Inventory::remove(anax::Entity::Id& id)
{
    for (auto& group: m_groups)
    {
        auto& set = group.second.entities;
        auto it = set.find(id.value());

        if (it == set.end())
            continue;

        delEntity(it, set);
    }
}

bool Components::Inventory::has(anax::Entity::Id& id)
{
    auto& set = m_groups["all"].entities;
    auto it = set.find(id.value());

    if (it == set.end())
        return false;

    return true;
}

const Components::Inventory::EntityCache& Components::Inventory::getAll()
{
    return m_groups["all"].entities;
}

void Components::Inventory::reset()
{
    for (auto const& name:
        {"all", "edible", "equippable", "resource", //basic groups
         "attack", "resistance"}) //automatic groups
    {
        m_groups[name] = Group{name};
    }
}

void Components::Inventory::delEntity(const EntityCache::iterator& it, EntityCache& where)
{
    where.erase(it);
    m_world.getEntity(*it).kill(); //goodbye !
}


bool Components::Map::passable(unsigned sX, unsigned sY, unsigned eX, unsigned eY)
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
        if (eX > Def::LMAPX || eY > Def::LMAPY)
            return false;

        XYToArray(eX, eY);

        if (eX > Def::MAPX || eY > Def::MAPY)
            return false;

        unsigned tile {eX+eY*Def::MAPX};

        unsigned tileNumber = obs[tile];
        return (tileNumber == 0); //Go look the line after "TILEARRAY obs" in this class.
    }
}

float Components::Map::LeastCostEstimate( void* nodeStart, void* nodeEnd )
{
    unsigned sX{}, sY{};
    NodeToXY(nodeStart, sX, sY);

    unsigned eX{}, eY{};
    NodeToXY(nodeEnd, eX, eY);


    unsigned rX{distance(sX, eX)}, rY{distance(sY, eY)};
    unsigned const estimated{rX+rY};

    return static_cast<float>(estimated);
}

void Components::Map::AdjacentCost( void* node, std::vector< micropather::StateCost > *neighbors )
{
    assert(neighbors && "Neighbords null !");

    static constexpr std::array<int,   8> dx   { 0,   2,   0 , -2,   1,   -1,    1,   -1    };
    static constexpr std::array<int,   8> dy   { 2,   0,  -2,   0,   1,   -1,   -1,    1    };
    static constexpr std::array<float, 8> cost { 2.f, 2.f, 2.f, 2.f, 1.5f, 1.5f, 1.5f, 1.5f };

    unsigned x{}, y{};
    NodeToXY( node, x, y );

    for (std::size_t i{}; i < cost.size(); ++i)
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
