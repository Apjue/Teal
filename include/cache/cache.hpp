#ifndef CACHE_HPP
#define CACHE_HPP

#pragma once

#include <unordered_map>
#include <utility>
#include "producers.hpp"
#include "util.hpp"
#include "classhash.hpp"

template<class T>
using DefaultCacheProducer = SharedPointerProducer<T>;

///
/// \class Cache
///
/// \brief wraps Producer<T>::create() objects
///
/// \note Producer must have a create() function
///

template<class Key, class T, class Producer = DefaultCacheProducer<T>>
class Cache
{
public:
    using ManagerType = decltype(Producer::create());
    using InternalCache = typename std::unordered_map<Key, ManagerType>;

    Cache() = default;
    ~Cache() = default;

    ManagerType get(const Key& k) const;

    template<class... Args>
    ManagerType add(const Key& k, Args&&... args);

protected:
    InternalCache m_objects;

    template<class... Args>
    typename InternalCache::iterator add_(const Key& k, Args&&... args);
};

///
/// \class CreateCache
///
/// \brief Acts like a normal cache, but create an object if
///        it didn't exist when using get()
///

template<class Key, class T, class Producer = DefaultCacheProducer<T>>
class CreateCache : private Cache<Key, T, Producer>
{
public:
    CreateCache() = default;
    ~CreateCache() = default;

    template<class... Args>
    ManagerType get(const Key& k, Args&&... args);

    using Cache::add;

private:
    using Cache::add_;
};

#include "cache.inl"

#endif // CACHE_HPP
