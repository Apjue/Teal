// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class K, class T, class P>
typename const Cache<K, T, P>::ManagerType& Cache<K, T, P>::get(const K& k) const
{
    auto it = m_objects.find(k);

    if (it == m_objects.end())
        return empty;
    else
        return it->second;
}

template<class K, class T, class P>
void Cache<K, T, P>::clear()
{
    m_objects.clear();
}

template<class K, class T, class P>
template<class... Args>
typename Cache<K, T, P>::ManagerType Cache<K, T, P>::add(const K& k, Args&&... args)
{
    return add_(k, std::forward<Args>(args)...)->second;
}

template<class K, class T, class P>
template<class... Args>
typename Cache<K, T, P>::InternalCache::iterator Cache<K, T, P>::add_(const K& k, Args&&... args)
{
    return m_objects.emplace(k, Producer::create<Args...>(std::forward<Args>(args)...)).first;
}

template<class K, class T, class P>
typename const Cache<K, T, P>::InternalCache& Cache<K, T, P>::getInternalCache() const
{
    return m_objects;
}

template<class K, class T, class P>
typename Cache<K, T, P>::InternalCache& Cache<K, T, P>::getInternalCache()
{
    return getInternalCache();
}


template<class K, class T, class P>
template<class... Args>
typename CreateCache<K, T, P>::ManagerType CreateCache<K, T, P>::get(const K& k, Args&&... args)
{
    auto it = m_objects.find(k);

    if (it == m_objects.end())
        return add(k, std::forward<Args>(args)...)->second;
    else
        return it->second;
}
