// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class T>
Maybe& Maybe<T>::operator=(T&& newT)
{
    m_res = newT;
    m_valid = true;
}

template<class T>
bool Maybe<T>::isValid() const noexcept
{
    return m_valid;
}

template<class T>
const T& Maybe<T>::get() const
{
    assert(m_valid && "Maybe is not valid !");
    return m_res;
}

template<class T>
T& Maybe<T>::get()
{
    assert(m_valid && "Maybe is not valid !");
    return m_res;
}
