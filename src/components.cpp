// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components.hpp"

namespace Components
{

void Inventory::add(const anax::Entity::Id& id)
{    
    auto entity = m_world.getEntity(id.value());
    assertItem(entity);

    m_groups["all"].add(id.value());

    //Add to basic groups
    if (entity.hasComponent<Items::Edible>())
        m_groups["edible"].add(id.value());

    if (entity.hasComponent<Items::Equippable>())
        m_groups["equippable"].add(id.value());

    if (entity.hasComponent<Items::Resource>())
        m_groups["resource"].add(id.value());
}

void Inventory::remove(const anax::Entity::Id& id)
{
    assertItem(m_world.getEntity(id.value()));

    for (auto& group: m_groups)
    {
        group.second.remove(id.value());
    }
}

bool Inventory::has(const anax::Entity::Id& id)
{
    assertItem(m_world.getEntity(id.value()));

    auto& group = m_groups["all"];
    auto it = group.find(id.value(), true);

    return (it == group.entities().end());
}

void Inventory::deactivate(const anax::Entity::Id& id)
{
    assertItem(m_world.getEntity(id.value()));

    for (auto& group: m_groups)
    {
        group.second.deactivate(id.value());
    }
}

void Inventory::activate(const anax::Entity::Id& id)
{
    assertItem(m_world.getEntity(id.value()));

    for (auto& group: m_groups)
    {
        group.second.activate(id.value());
    }
}

const Inventory::EntityCache& Components::Inventory::getAll()
{
    return m_groups["all"].entities();
}

void Inventory::reset()
{
    for (auto const& name:
        {"all", "edible", "equippable", "resource"})
    {
        m_groups[name] = Group{name};
    }
}


bool Map::passable(unsigned sX, unsigned sY, unsigned eX, unsigned eY)
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

float Map::LeastCostEstimate( void* nodeStart, void* nodeEnd )
{
    unsigned sX{}, sY{};
    NodeToXY(nodeStart, sX, sY);

    unsigned eX{}, eY{};
    NodeToXY(nodeEnd, eX, eY);


    unsigned rX{distance(sX, eX)}, rY{distance(sY, eY)};
    unsigned const estimated{rX+rY};

    return static_cast<float>(estimated);
}

void Map::AdjacentCost( void* node, std::vector< micropather::StateCost > *neighbors )
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

} //namespace Components
