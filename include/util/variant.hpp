// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef VARIANT_HPP
#define VARIANT_HPP

#include <iostream>
#include <utility>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <string>
#include "staticmax.hpp"

template<typename... Ts>
struct VariantHelper;

template<typename F, typename... Ts>
struct VariantHelper<F, Ts...>
{
    inline static void destroy(const std::type_index& id, void* data);
    inline static void move(const std::type_index& old_t, void* old_v, void* new_v);
    inline static void copy(const std::type_index& old_t, const void* old_v, void* new_v);
};

template<> struct VariantHelper<>
{
    inline static void destroy(const std::type_index& id, void * data) {}
    inline static void move(const std::type_index& old_t, void * old_v, void * new_v) {}
    inline static void copy(const std::type_index& old_t, const void * old_v, void * new_v) {}
};

template<typename... Ts>
class Variant
{
public:
    inline Variant();
    inline ~Variant();

    inline Variant(const Variant<Ts...>& other);
    inline Variant(Variant<Ts...>&& other);


    inline Variant<Ts...>& operator= (const Variant<Ts...>& other);
    inline Variant<Ts...>& operator= (Variant<Ts...>&& other);


    template<typename T>
    inline bool is();

    inline bool valid();


    template<typename T, typename... Args>
    inline void set(Args&&... args);

    template<typename T>
    inline T& get();

private:
    static const size_t data_size = StaticMax<sizeof(Ts)...>::value;
    static const size_t data_align = StaticMax<alignof(Ts)...>::value;

    using Data = typename std::aligned_storage<data_size, data_align>::type;
    using Helper = VariantHelper<Ts...>;

    static inline std::type_index invalid_type();

    std::type_index m_typeid;
    Data m_data;
};

#include "variant.inl"

#endif // VARIANT_HPP
