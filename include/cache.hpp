#ifndef CACHE_HPP
#define CACHE_HPP

#pragma once

#include <map>
#include <utility>
#include "util.hpp"

///
/// \brief The CreateCache class
///
/// returns the T object if it exists or construct it
/// with args provided, and returns it.
///

template<class Key, class T>
class CreateCache
{
public:
    CreateCache() = default;
    ~CreateCache() = default;

    template<class... Args>
    T get(Key k, Args&&... args)
    {
        auto it = m_objects.find(k);

        if (it == m_objects.end()) //Not found
            m_objects[k] = T{std::forward(args)...}; //Create

        return m_objects[k];
    }

private:
    std::map<Key, T> m_objects;
};

///
/// \brief The MaybeCache class
///
/// Returns a Maybe<T> object
///
/// \see The Maybe class (util.hpp)
///

template<class Key, class T>
class MaybeCache
{
public:
    MaybeCache() = default;
    ~MaybeCache() = default;

    Maybe<T> get(Key k)
    {
        auto it = m_objects.find(k);

        if (it == m_objects.end()) //Not found
            return Maybe<T>{};

        return Maybe<T> { m_objects[k] };
    }

private:
    std::map<Key, T> m_objects;
};

#endif // CACHE_HPP
