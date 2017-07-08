// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CACHE_HPP
#define CACHE_HPP

#pragma once

#include <unordered_map>
#include <utility>
#include <type_traits>
#include "producers.hpp"
#include "util/util.hpp"
#include "util/classhash.hpp"

template<class T>
using DefaultCacheProducer = SharedPointerProducer<T>;

///
/// \class Cache
///
/// \brief Wraps ManagerType objects
///        If object not found, m_empty ManagerType object is returned.
///

template<class K, class T, class Producer = DefaultCacheProducer<T>, class Container = std::unordered_map<K, decltype(Producer::create())>>
class Cache
{
public:
    using InternalCache = Container;
    using Key = typename Container::key_type;
    using ManagerType = typename Container::mapped_type;

    static_assert(std::is_same<K, Key>::value, "Keys types are different !");
    static_assert(std::is_same<T, typename Producer::Type>::value, "Objects types are different !");
    static_assert(std::is_same<decltype(Producer::create()), ManagerType>::value, "Objects types are different !");

    Cache(ManagerType empty);
    ~Cache() = default;

    ///
    /// \fn get
    ///
    /// \return The item with Key k, or m_empty if not found
    ///

    const ManagerType& get(const Key& k) const;

    template<class... Args>
    ManagerType add(const Key& k, Args&&... args);

    inline void clear();

    inline const InternalCache& getUnderlyingCache() const;
    inline InternalCache& getUnderlyingCache();

protected:
    InternalCache m_objects;
    const ManagerType m_empty; // Value to return if not found

    template<class... Args>
    typename InternalCache::iterator add_(const Key& k, Args&&... args);
};

#include "cache.inl"

#endif // CACHE_HPP
