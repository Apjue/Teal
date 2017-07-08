// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

namespace Detail
{

template<class F, class... Ts>
void VariantHelper<F, Ts...>::destroy(const std::type_index& id, void* data)
{
    if (id == typeid(F))
        static_cast<F*>(data)->~F();

    else
        VariantHelper<Ts...>::destroy(id, data);
}

template<class F, class... Ts>
void VariantHelper<F, Ts...>::move(const std::type_index& old_t, void* old_v, void* new_v)
{
    if (old_t == typeid(F))
        new (new_v) F(std::move(*static_cast<F*>(old_v)));

    else
        VariantHelper<Ts...>::move(old_t, old_v, new_v);
}

template<class F, class... Ts>
void VariantHelper<F, Ts...>::copy(const std::type_index& old_t, const void* old_v, void* new_v)
{
    if (old_t == typeid(F))
        new (new_v) F(*static_cast<const F*>(old_v));

    else
        VariantHelper<Ts...>::copy(old_t, old_v, new_v);
}

} // namespace Detail

template<class... Ts>
Variant<Ts...>::~Variant()
{
    Helper::destroy(m_typeid, &m_data);
}


template<class... Ts>
Variant<Ts...>::Variant(const Variant<Ts...>& other) : m_typeid(other.m_typeid)
{
    Helper::copy(other.m_typeid, &other.m_data, &m_data);
}

template<class... Ts>
Variant<Ts...>::Variant(Variant<Ts...>&& other) : m_typeid(other.m_typeid)
{
    Helper::move(other.m_typeid, &other.m_data, &m_data);
}


template<class... Ts>
Variant<Ts...>& Variant<Ts...>::operator= (const Variant<Ts...>& other)
{
    Helper::copy(other.m_typeid, &other.m_data, &m_data);
    return *this;
}

template<class... Ts>
Variant<Ts...>& Variant<Ts...>::operator= (Variant<Ts...>&& other)
{
    Helper::move(other.m_typeid, &other.m_data, &m_data);
    return *this;
}


template<class... Ts>
template<class T>
bool Variant<Ts...>::is() const
{
    return m_typeid == typeid(T);
}

template<class... Ts>
bool Variant<Ts...>::valid() const
{
    return m_typeid != invalid_type();
}


template<class... Ts>
template<class T, class... Args, class>
void Variant<Ts...>::set(Args&&... args)
{
    if (valid())
        Helper::destroy(m_typeid, &m_data);

    new (&m_data) T(std::forward<Args>(args)...);
    m_typeid = typeid(T);
}

template<class... Ts>
template<class T, class>
const T& Variant<Ts...>::get() const
{
    TealAssert(valid(), "Uninitialized variant !");

    if (m_typeid == typeid(T))
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

    if (m_typeid == typeid(T))
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
        Helper::Destroy(m_typeid, &m_data);

    m_typeid = invalid_type();
}

template<class... Ts>
std::type_index Variant<Ts...>::invalid_type()
{
    return typeid(void);
}
