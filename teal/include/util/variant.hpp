// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef VARIANT_HPP
#define VARIANT_HPP

#include <utility>
#include <type_traits>
#include "util/assert.hpp"
#include "util/isoneof.hpp"
#include "util/indexof.hpp"

namespace VariantDetail
{

template<class... Ts>
struct VariantHelper;

template<class Union, class T, class... Ts>
struct VariantHelper<Union, T, Ts...>
{
    inline static void destroy(std::size_t index, Union* data);
    inline static void move(std::size_t index, Union* oldValue, Union* newValue);
    inline static void copy(std::size_t index, const Union* oldValue, Union* new_v);
};

template<class Union>
struct VariantHelper<Union>
{
    inline static void destroy(std::size_t index, Union* data) {}
    inline static void move(std::size_t index, Union* oldValue, Union* newValue) {}
    inline static void copy(std::size_t index, const Union* oldValue, Union* newValue) {}
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
    using Helper = VariantDetail::VariantHelper<Data, Ts...>;

    std::size_t m_index {};
    Data m_data;
};

#include "variant.inl"

#endif // VARIANT_HPP
