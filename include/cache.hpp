#ifndef CACHE_HPP
#define CACHE_HPP

#include <map>

template<class Key, class T>
class Cache
{
public:
    Cache() = default;
    ~Cache() = default;

    template<class... Args>
    T get(Key k, Args... args)
    {
        auto it = m_objects.find(k);

        if (it == m_objects.end()) //Not found
        {
            m_objects[k] = T{args...}; //Create
            return m_objects[k];
        }
        return m_objects[k];
    }

private:
    std::map<Key, T> m_objects;
};

#endif // CACHE_HPP
