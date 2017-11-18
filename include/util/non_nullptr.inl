// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <assert.h>

template<class T>
non_nullptr<T>::non_nullptr(T* ptr) noexcept
    : m_ptr(ptr)
{
    TealAssert(ptr, "Ptr is null !");
}

template<class T>
non_nullptr& non_nullptr<T>::operator=(T* ptr) noexcept
{
    TealAssert(ptr, "Ptr is null !");
    m_ptr = ptr;
    return *this;
}

template<class T>
bool non_nullptr<T>::operator==(const non_nullptr<T>& other) const noexcept
{
    return m_ptr == other.m_ptr;
}

template<class T>
T* non_nullptr<T>::get() const noexcept
{
    return m_ptr;
}

template<class T>
T& non_nullptr<T>::operator*() const noexcept
{
    return *m_ptr;
}

template<class T>
T* non_nullptr<T>::operator->() const noexcept
{
    return m_ptr;
}
