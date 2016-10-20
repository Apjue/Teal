#ifndef CACHE_HPP
#define CACHE_HPP

#pragma once

#include <unordered_map>
#include <utility>
#include "producers.hpp"
#include "util/util.hpp"
#include "util/classhash.hpp"

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
    using InternalCache = std::unordered_map<Key, ManagerType>;

    static ManagerType empty; // Value to return if not found

    Cache() = default;
    ~Cache() = default;

    ///
    /// \fn get
    ///
    /// \return The item with Key k, or empty if not found
    ///

    const ManagerType& get(const Key& k) const;

    template<class... Args>
    ManagerType add(const Key& k, Args&&... args);

    inline void clear();

    inline const InternalCache& getInternalCache() const;
    inline InternalCache& getInternalCache();

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

    using Cache::empty;

    CreateCache() = default;
    ~CreateCache() = default;

    template<class... Args>
    ManagerType get(const Key& k, Args&&... args);

    using Cache::add;
    using Cache::clear;

protected:
    using Cache::add_;
};

#include "cache.inl"

#endif // CACHE_HPP
