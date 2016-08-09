// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ENTITYTEMPLATE_HPP
#define ENTITYTEMPLATE_HPP

#include <memory>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <assert.h>
#include <anax/Component.hpp>

class EntityTemplate
{
    using ComponentArray = std::vector<std::shared_ptr<anax::Component>>;

public:
    EntityTemplate() = default;
    ~EntityTemplate() = default;

    template <typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        static_assert(std::is_base_of<anax::Component, T>(), "T is not a component");

        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        m_components.push_back(component);

        return *component;
    }

    template <typename T>
    void removeComponent()
    {
        static_assert(std::is_base_of<anax::Component, T>(), "T is not a component");

        auto it = findComponent<T>();

        if (it != m_components.end())
            m_components.erase(it);
    }

    template <typename T>
    T& getComponent()
    {
        static_assert(std::is_base_of<anax::Component, T>(), "T is not a component");

        auto it = findComponent<T>();

        if (it == m_components.end())
            assert(!"Entity doesn't have T");

        return *it;
    }

    template <typename T>
    bool hasComponent() const
    {
        static_assert(std::is_base_of<anax::Component, T>(), "T is not a component");

        return (findComponent<T>() != m_components.end());
    }

private:
    ComponentArray m_components;

    template<class T>
    ComponentArray::iterator findComponent() const
    {
        return std::find_if(m_components.begin(), m_components.end(),
                            [](auto&& t)
        {
            return (dynamic_cast<T>(t.get()) != nullptr);
        });
    }
};

#endif // ENTITYTEMPLATE_HPP
