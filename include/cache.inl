// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class Key, class T, class Producer>
typename Cache<Key, T, Producer>::ManagerType Cache<Key, T, Producer>::get(const Key& k) const
{
    auto it = m_objects.find(k);

    if (it == m_objects.end())
        return ManagerType {};
    else
        return it->second;
}

template<class Key, class T, class Producer>
template<class... Args>
typename Cache<Key, T, Producer>::ManagerType Cache<Key, T, Producer>::add(const Key& k, Args&&... args)
{
    return add_(k, std::forward<Args>(args)...)->second;
}

template<class Key, class T, class Producer>
template<class... Args>
typename Cache<Key, T, Producer>::InternalCache::iterator Cache<Key, T, Producer>::add_(const Key& k, Args&&... args)
{
    return m_objects.emplace(k, Producer::create<Args...>(std::forward<Args>(args)...)).first;
}

template<class Key, class T, class Producer>
template<class... Args>
typename CreateCache<Key, T, Producer>::ManagerType CreateCache<Key, T, Producer>::get(const Key& k, Args&&... args)
{
    auto it = m_objects.find(k);

    if (it == m_objects.end())
        return add(k, std::forward<Args>(args)...)->second;
    else
        return it->second;
}
