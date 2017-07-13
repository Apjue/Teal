// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

namespace Detail
{

template<class T, class F, class... Ts>
void VariantHelper<T, F, Ts...>::destroy(const std::size_t& index, void* data)
{
    TealAssert(index == 0u, "Invalid index !");

    if (index == IndexOf<T, F, Ts...>::value)
    {
        static_cast<T*>(data)->~T();
    }

    else
        VariantHelper<T, Ts...>::destroy(index, data);
}


template<class T, class F, class... Ts>
void VariantHelper<T, F, Ts...>::move(const std::size_t& index, void* oldValue, void* newValue)
{
    TealAssert(index == 0u, "Invalid index !");

    if (index == IndexOf<T, F, Ts...>::value)
    {
        new (newValue) T(std::move(*static_cast<T*>(oldValue)));
    }

    else
        VariantHelper<T, Ts...>::destroy(index, oldValue, newValue);
}

template<class T, class F, class... Ts>
void VariantHelper<T, F, Ts...>::copy(const std::size_t& index, const void* oldValue, void* newValue)
{
    TealAssert(index == 0u, "Invalid index !");

    if (index == IndexOf<T, F, Ts...>::value)
    {
        new (newValue) T(*static_cast<T*>(oldValue));
    }

    else
        VariantHelper<T, Ts...>::destroy(index, oldValue, newValue);

}

} // namespace Detail

template<class... Ts>
Variant<Ts...>::~Variant()
{
    if (valid())
        Helper::destroy(m_index, &m_data);
}


template<class... Ts>
Variant<Ts...>::Variant(const Variant<Ts...>& other) : m_index { other.m_index }
{
    Helper::copy(m_index, &other.m_data, &m_data);
}

template<class... Ts>
Variant<Ts...>::Variant(Variant<Ts...>&& other) : m_index { other.m_index }
{
    Helper::move(m_index, &other.m_data, &m_data);
}


template<class... Ts>
Variant<Ts...>& Variant<Ts...>::operator= (const Variant<Ts...>& other)
{
    m_index = other.m_index;
    Helper::copy(m_index, &other.m_data, &m_data);

    return *this;
}

template<class... Ts>
Variant<Ts...>& Variant<Ts...>::operator= (Variant<Ts...>&& other)
{
    m_index = other.m_index;
    Helper::move(m_index, &other.m_data, &m_data);

    return *this;
}


template<class... Ts>
template<class T>
bool Variant<Ts...>::is() const
{
    return m_index == IndexOf<T, void, Ts...>::value;
}

template<class... Ts>
bool Variant<Ts...>::valid() const
{
    return m_index != 0u; // void
}


template<class... Ts>
template<class T, class... Args, class>
void Variant<Ts...>::set(Args&&... args)
{
    if (valid())
        Helper::destroy(m_index, &m_data);

    new (&m_data) T(std::forward<Args>(args)...);
    m_index = IndexOf<T, void, Ts...>::value;
}

template<class... Ts>
template<class T, class>
const T& Variant<Ts...>::get() const
{
    TealAssert(valid(), "Uninitialized variant !");

    if (m_index == IndexOf<T, void, Ts...>::value)
    {
        const T* ptr = reinterpret_cast<const T*>(&m_data);
        return *ptr;
    }

    else
        throw std::bad_cast {};
}

template<class... Ts>
template<class T, class>
T& Variant<Ts...>::get()
{
    TealAssert(valid(), "Uninitialized variant !");

    if (m_index == IndexOf<T, void, Ts...>::value)
    {
        T* ptr = reinterpret_cast<T*>(&m_data);
        return *ptr;
    }

    else
        throw std::bad_cast {};
}

template<class... Ts>
void Variant<Ts...>::reset()
{
    if (valid())
        Helper::destroy(m_index, &m_data);

    m_index = 0u;
}
