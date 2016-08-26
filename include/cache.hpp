#ifndef CACHE_HPP
#define CACHE_HPP

#pragma once

#include <unordered_map>
#include <utility>
#include <memory>
#include "util.hpp"

///
/// \class DefaultProducer
///

template<class T>
struct DefaultProducer
{
    ///
    /// \fn operator()
    ///
    /// \param args Arguments used to create the object
    ///
    /// \return a T* object
    ///

    template<class... Args>
    T* operator()(Args&&... args)
    {
        std::unique_ptr res = std::make_unique<T>(std::forward<Args>(args)...);
        return res.release();
    }
};

///
/// \class CreateCache
///
/// \brief wraps std::shared_ptr<T> objects
///

template<class Key, class T, class ProduceType = DefaultProducer<T>>
class CreateCache
{
    using InternalCache = typename std::unordered_map<Key, std::shared_ptr<T>>;

public:
    CreateCache() = default;
    ~CreateCache() = default;

    template<class... Args>
    std::shared_ptr<T> get(const Key& k, Args&&... args)
    {
        auto it = m_objects.find(k);

        if (it == m_objects.end())
            return add(k, std::forward<Args>(args)...)->second;
        else
            return it->second;
    }

    template<class... Args>
    std::shared_ptr<T> add(const Key& k, Args&&... args)
    {
        return add_(k, std::forward<Args>(args)...)->second;
    }

private:
    InternalCache m_objects;
    ProduceType prod;

    template<class... Args>
    typename InternalCache::iterator add_(const Key& k, Args&&... args)
    {
        return m_objects.emplace(k, prod(std::forward<Args>(args)...)).second;
    }
};

///
/// \class Cache
///
/// \brief wraps std::shared_ptr<T> objects
///

template<class Key, class T, class ProduceType = DefaultProducer<T>>
class Cache
{
    using InternalCache = typename std::unordered_map<Key, std::shared_ptr<T>>;

public:
    Cache() = default;
    ~Cache() = default;

    template<class... Args>
    std::shared_ptr<T> get(const Key& k, Args&&... args)
    {
        auto it = m_objects.find(k);

        if (it == m_objects.end())
            return std::shared_ptr {};
        else
            return it->second;
    }

    template<class... Args>
    std::shared_ptr<T> add(const Key& k, Args&&... args)
    {
        return add_(k, std::forward<Args>(args)...)->second;
    }

private:
    InternalCache m_objects;
    ProduceType prod;

    template<class... Args>
    typename InternalCache::iterator add_(const Key& k, Args&&... args)
    {
        return m_objects.emplace(k, prod(std::forward<Args>(args)...)).second;
    }
};

#endif // CACHE_HPP
