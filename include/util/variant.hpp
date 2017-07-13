// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef VARIANT_HPP
#define VARIANT_HPP

#include <utility>
#include <type_traits>
#include "assert.hpp"
#include "isoneof.hpp"
#include "indexof.hpp"

namespace Detail
{

template<class... Ts>
struct VariantHelper;

template<class T, class F, class... Ts>
struct VariantHelper<T, F, Ts...>
{
    inline static void destroy(const std::size_t& index, void* data);
    inline static void move(const std::size_t& index, void* oldValue, void* newValue);
    inline static void copy(const std::size_t& index, const void* oldValue, void* new_v);
};

template<class T>
struct VariantHelper<T>
{
    inline static void destroy(const std::size_t& index, void* data) {}
    inline static void move(const std::size_t& index, void* oldValue, void* newValue) {}
    inline static void copy(const std::size_t& index, const void* oldValue, void* newValue) {}
};

} // namespace Detail

template<class... Ts>
class Variant
{
public:
    static_assert(sizeof...(Ts) > 1, "Variant must have at least 2 different types");

    Variant() = default;
    inline ~Variant();

    inline Variant(const Variant<Ts...>& other);
    inline Variant(Variant<Ts...>&& other);


    inline Variant<Ts...>& operator= (const Variant<Ts...>& other);
    inline Variant<Ts...>& operator= (Variant<Ts...>&& other);


    template<class T>
    inline bool is() const;

    inline bool valid() const;


    template<class T, class... Args, class = typename std::enable_if<IsOneOf<T, Ts...>::value>::type>
    inline void set(Args&&... args);

    template<class T, class = typename std::enable_if<IsOneOf<T, Ts...>::value>::type>
    inline const T& get() const;

    template<class T, class = typename std::enable_if<IsOneOf<T, Ts...>::value>::type>
    inline T& get();

    inline void reset();

private:
    using Data = typename std::aligned_union<0, Ts...>::type;
    using Helper = Detail::VariantHelper<void, Ts...>;

    std::size_t m_index {};
    Data m_data;
};

#include "variant.inl"

#endif // VARIANT_HPP
