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
