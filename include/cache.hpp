#ifndef CACHE_HPP
#define CACHE_HPP

#pragma once

#include <unordered_map>
#include <utility>
#include "producers.hpp"
#include "util.hpp"

///
/// \class CreateCache
///
/// \brief wraps ProduceType<T>::operator() objects
///
/// \note ProduceType must have an operator() function
///

template<class Key, class T, class ProduceType = DefaultProducer<T>>
class CreateCache
{
    using ManagerType = decltype(ProduceType::create());
    using InternalCache = typename std::unordered_map<Key, ManagerType>;

public:
    CreateCache() = default;
    ~CreateCache() = default;

    template<class... Args>
    ManagerType get(const Key& k, Args&&... args)
    {
        auto it = m_objects.find(k);

        if (it == m_objects.end())
            return add(k, std::forward<Args>(args)...)->second;
        else
            return it->second;
    }

    template<class... Args>
    ManagerType add(const Key& k, Args&&... args)
    {
        return add_(k, std::forward<Args>(args)...)->second;
    }

private:
    InternalCache m_objects;
    ProduceType prod;

    template<class... Args>
    typename InternalCache::iterator add_(const Key& k, Args&&... args)
    {
        return m_objects.emplace(k, ProduceType::create(std::forward<Args>(args)...)).first;
    }
};

///
/// \class Cache
///
/// \brief wraps ProduceType<T>::operator() objects
///
/// \note ProduceType must have an operator() function
///

template<class Key, class T, class ProduceType = DefaultProducer<T>>
class Cache
{
    using ManagerType = decltype(ProduceType::create());
    using InternalCache = typename std::unordered_map<Key, ManagerType>;

public:
    Cache() = default;
    ~Cache() = default;

    ManagerType get(const Key& k) const
    {
        auto it = m_objects.find(k);

        if (it == m_objects.end())
            return ManagerType {};
        else
            return it->second;
    }

    template<class... Args>
    ManagerType add(const Key& k, Args&&... args)
    {
        return add_(k, std::forward<Args>(args)...)->second;
    }

private:
    InternalCache m_objects;

    template<class... Args>
    typename InternalCache::iterator add_(const Key& k, Args&&... args)
    {
        return m_objects.emplace(k, ProduceType::create(std::forward<Args>(args)...)).first;
    }
};

#endif // CACHE_HPP
