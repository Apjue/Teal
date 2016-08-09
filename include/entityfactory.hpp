// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include <unordered_map>
#include <string>

#include <anax/Entity.hpp>
#include <anax/World.hpp>
#include "entitytemplate.hpp"

class EntityFactory
{
    using EntityRefArray = std::unordered_map<std::string, EntityTemplate>;

public:
    EntityFactory() = default;
    ~EntityFactory() = default;

    void provide(anax::World* world)
    {
        m_world = world;
    }

    void addModel(const std::string& name, const EntityTemplate& model);

    template<class... Components>
    anax::Entity createEntity(const std::string& name)
    {
        auto it = m_entities.find(name);

        if (it == m_entities.end())
            return {};

        assert(m_world && "No world has been provided");
        anax::Entity e = m_world->createEntity();


    }

private:
    static EntityRefArray m_entities;
    static anax::World* m_world;

    template<class Component1, class Component2, class... Rest>
    void addComponents(anax::Entity& e, const std::string& name)
    {

    }
    template<class Component>
    void addComponent(anax::Entity& e)
    {
        e.addComponent<Component>();
        ;
    }
};

#endif // ENTITYFACTORY_HPP
