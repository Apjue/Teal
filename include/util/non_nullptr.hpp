// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef NON_NULLPTR_HPP
#define NON_NULLPTR_HPP

///
/// \class non_nullptr
///
/// \brief This class ensure that its pointer never points to nullptr
///
/// \note It doesn't manage memory for you.
///
/// \example non_nullptr<int> intPtr(new int);
///

template<class T>
class non_nullptr
{
    non_nullptr(decltype(nullptr)) = delete;
    non_nullptr(int) = delete;

    non_nullptr& operator=(decltype(nullptr)) noexcept = delete;
    non_nullptr& operator=(int) noexcept = delete;

public:
    using pointer = T*;
    using element_type = T;

    non_nullptr(T* ptr) noexcept;
    non_nullptr& operator=(T* ptr) noexcept;
    bool operator==(const non_nullptr<T>& other) const noexcept;

    T* get() const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;

private:
    T* m_ptr;
};

#include "non_nullptr.inl"

#endif // NON_NULLPTR_HPP
