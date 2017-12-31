// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

namespace Detail
{

template<class Union, class T, class... Ts>
void VariantHelper<Union, T, Ts...>::destroy(std::size_t index, Union* data)
{
    if (index == 0u)
        reinterpret_cast<T*>(data)->~T();

    else
    {
        --index;
        VariantHelper<Union, Ts...>::destroy(index, data);
    }
}


template<class Union, class T, class... Ts>
void VariantHelper<Union, T, Ts...>::move(std::size_t index, Union* oldValue, Union* newValue)
{
    if (index == 0u)
        new (newValue) T(std::move(*reinterpret_cast<T*>(oldValue)));

    else
    {
        --index;
        VariantHelper<Union, Ts...>::move(index, oldValue, newValue);
    }
}

template<class Union, class T, class... Ts>
void VariantHelper<Union, T, Ts...>::copy(std::size_t index, const Union* oldValue, Union* newValue)
{
    if (index == 0u)
        new (newValue) T(*reinterpret_cast<const T*>(oldValue));

    else
    {
        --index;
        VariantHelper<Union, Ts...>::copy(index, oldValue, newValue);
    }

}

} // namespace Detail

template<class... Ts>
Variant<Ts...>::~Variant()
{
    if (valid())
        Helper::destroy(m_index - 1u, &m_data);
}


template<class... Ts>
Variant<Ts...>::Variant(const Variant<Ts...>& other) : m_index { other.m_index }
{
    if (valid())
        Helper::copy(m_index - 1u, &other.m_data, &m_data);
}

template<class... Ts>
Variant<Ts...>::Variant(Variant<Ts...>&& other) : m_index { other.m_index }
{
    if (valid())
        Helper::move(m_index - 1u, &other.m_data, &m_data);
}


template<class... Ts>
Variant<Ts...>& Variant<Ts...>::operator= (const Variant<Ts...>& other)
{
    if (valid())
        Helper::destroy(m_index - 1u, &m_data);

    m_index = other.m_index;

    if (valid())
        Helper::copy(m_index - 1u, &other.m_data, &m_data);

    return *this;
}

template<class... Ts>
Variant<Ts...>& Variant<Ts...>::operator= (Variant<Ts...>&& other)
{
    if (valid())
        Helper::destroy(m_index - 1u, &m_data);

    m_index = other.m_index;

    if (valid())
        Helper::move(m_index - 1u, &other.m_data, &m_data);

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
        Helper::destroy(m_index - 1u, &m_data);

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
        Helper::destroy(m_index - 1u, &m_data);

    m_index = 0u;
}
