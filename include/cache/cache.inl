// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class K, class T, class P, class C>
Cache<K, T, P, C>::Cache(ManagerType empty) : m_empty { empty } {}

template<class K, class T, class P, class C>
const typename Cache<K, T, P, C>::ManagerType& Cache<K, T, P, C>::get(const Key& k) const
{
    auto it = m_objects.find(k);

    if (it == m_objects.end())
        return m_empty;
    else
        return it->second;
}

template<class K, class T, class P, class C>
void Cache<K, T, P, C>::clear()
{
    m_objects.clear();
}

template<class K, class T, class P, class C>
template<class... Args>
typename Cache<K, T, P, C>::ManagerType Cache<K, T, P, C>::add(const Key& k, Args&&... args)
{
    return add_(k, std::forward<Args>(args)...)->second;
}

template<class K, class T, class P, class C>
template<class... Args>
typename Cache<K, T, P, C>::InternalCache::iterator Cache<K, T, P, C>::add_(const Key& k, Args&&... args)
{
    return m_objects.emplace(k, P::template create<Args...>(std::forward<Args>(args)...)).first;
}

template<class K, class T, class P, class C>
const typename Cache<K, T, P, C>::InternalCache& Cache<K, T, P, C>::getUnderlyingCache() const
{
    return m_objects;
}

template<class K, class T, class P, class C>
typename Cache<K, T, P, C>::InternalCache& Cache<K, T, P, C>::getUnderlyingCache()
{
    return m_objects;
}
