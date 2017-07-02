// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<typename F, typename... Ts>
void VariantHelper<F, Ts...>::destroy(const std::type_index& id, void* data)
{
    if (id == typeid(F))
        static_cast<F*>(data)->~F();

    else
        VariantHelper<Ts...>::destroy(id, data);
}

template<typename F, typename... Ts>
void VariantHelper<F, Ts...>::move(const std::type_index& old_t, void* old_v, void* new_v)
{
    if (old_t == typeid(F))
        new (new_v) F(std::move(*static_cast<F*>(old_v)));

    else
        VariantHelper<Ts...>::move(old_t, old_v, new_v);
}

template<typename F, typename... Ts>
void VariantHelper<F, Ts...>::copy(const std::type_index& old_t, const void* old_v, void* new_v)
{
    if (old_t == typeid(F))
        new (new_v) F(*static_cast<const F*>(old_v));

    else
        VariantHelper<Ts...>::copy(old_t, old_v, new_v);
}


template<typename... Ts>
Variant<Ts...>::Variant() : m_typeid(invalid_type()) {}

template<typename... Ts>
Variant<Ts...>::~Variant()
{
    Helper::destroy(m_typeid, &m_data);
}


template<typename... Ts>
Variant<Ts...>::Variant(const Variant<Ts...>& other) : m_typeid(other.m_typeid)
{
    Helper::copy(other.m_typeid, &other.m_data, &m_data);
}

template<typename... Ts>
Variant<Ts...>::Variant(Variant<Ts...>&& other) : m_typeid(other.m_typeid)
{
    Helper::move(other.m_typeid, &other.m_data, &m_data);
}


template<typename... Ts>
Variant<Ts...>& Variant<Ts...>::operator= (const Variant<Ts...>& other)
{
    Helper::copy(other.m_typeid, &other.m_data, &m_data);
    return *this;
}

template<typename... Ts>
Variant<Ts...>& Variant<Ts...>::operator= (Variant<Ts...>&& other)
{
    Helper::move(other.m_typeid, &other.m_data, &m_data);
    return *this;
}


template<typename... Ts>
template<typename T>
bool Variant<Ts...>::is()
{
    return m_typeid == typeid(T);
}

template<typename... Ts>
bool Variant<Ts...>::valid()
{
    return m_typeid != invalid_type();
}


template<typename... Ts>
template<typename T, typename... Args>
void Variant<Ts...>::set(Args&&... args)
{
    Helper::destroy(m_typeid, &m_data);

    new (&m_data) T(std::forward<Args>(args)...);
    m_typeid = typeid(T);
}

template<typename... Ts>
template<typename T>
T& Variant<Ts...>::get()
{
    if (m_typeid == typeid(T))
    {
        T* ptr = static_cast<T*>(&m_data);
        TealAssert(ptr, "Variant: Cannot cast Aligned Storage to T");

        return *ptr;
    }

    else
        throw std::bad_cast {};
}

template<typename... Ts>
std::type_index Variant<Ts...>::invalid_type()
{
    return typeid(void);
}
