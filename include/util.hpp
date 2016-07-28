// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UTIL_HPP
#define UTIL_HPP

#include <QGraphicsPixmapItem>
#include <QRect>
#include <QPainter>
#include <QPixmap>
#include <QJsonDocument>
#include <QFile>

#include <assert.h>
#include <array>
#include "chrono.hpp"
#include "vector2.hpp"

///
/// \brief The non_nullptr class
///
/// This class ensure that its pointer never points to nullptr
///
/// \note It doesn't manage memory for you.
///
/// \example non_nullptr<int> intPtr(new int);
///

template<class T>
class non_nullptr
{
public:
    using pointer = T*;
    using element_type = T;

    non_nullptr(decltype(nullptr)) = delete;
    non_nullptr(int) = delete;

    non_nullptr& operator = (decltype(nullptr)) noexcept = delete;
    non_nullptr& operator = (int) noexcept = delete;

    non_nullptr(T* ptr) noexcept
    : m_ptr(ptr)
    {
        assert(ptr);
    }

    non_nullptr& operator = (T* ptr) noexcept
    {
        assert(ptr);
        this->m_ptr = ptr;
        return *this;
    }

    bool operator==(const non_nullptr<T>& other) const
    {
        return m_ptr == other.m_ptr;
    }

    T* get() const noexcept { return m_ptr; }
    T& operator*() const noexcept { return *m_ptr; }
    T* operator->() const noexcept { return m_ptr; }

private:
    T* m_ptr;
};

namespace Def
{

constexpr unsigned MAPX           {15u}; //For arrays.
constexpr unsigned MAPY           {8u};

constexpr unsigned TILEARRAYSIZE  {MAPX*MAPY};

constexpr unsigned LMAPX          {14u}; //For logic
constexpr unsigned LMAPY          {14u};

constexpr unsigned MAPXSIZE       {512u};
constexpr unsigned MAPYSIZE       {256u};

constexpr unsigned BUTTONSXSIZE   {MAPXSIZE};
constexpr unsigned BUTTONSYSIZE   {90u};

constexpr unsigned TILEXSIZE      {64u};
constexpr unsigned TILEYSIZE      {32u};

constexpr unsigned TILEGXSIZE     {TILEXSIZE / 2};
constexpr unsigned TILEGYSIZE     {TILEYSIZE / 2};

constexpr unsigned MAXPOSINTILE   {4u};
constexpr unsigned MAXGXPOSINTILE {TILEGXSIZE / MAXPOSINTILE};
constexpr unsigned MAXGYPOSINTILE {TILEGYSIZE / MAXPOSINTILE};

constexpr Miliseconds MAXFPS      {100};

constexpr int MAXDIR              {4};

}

using TILEARRAY = std::array<unsigned, Def::TILEARRAYSIZE>;

inline unsigned dToU(double d)
{
    return static_cast<unsigned>(d);
}

template<class T>
inline T distance(const T& x, const T& y)
{
    return (x > y) ? x - y : y - x;
}

using AbsTile = Vector2u; // Absolute Tile (0-15, 0-8)
using DiffTile = Vector2i; // Difference Tile (can be negative or positive)

///
/// \brief setTextureRect
///
/// Used to apply a texture rect of a texture on a Pixmap Item.
///
/// \see Animation System (systems.hpp/cpp)
///

extern void setTextureRect(QGraphicsPixmapItem& item, const QPixmap& texture, const QRect& rect);

///
/// \brief The Maybe class
///
/// It contains a T object or an invalid T object
/// Check isValid() before using get()
///

template<class T>
class Maybe
{
    Maybe(Maybe&) = delete;
    Maybe& operator=(const Maybe&) = delete;
    bool operator==(const Maybe&) = delete;

public:
    Maybe() = default;
    Maybe(T&& t) : m_res{t}, m_valid{true} {}
    ~Maybe() = default;

    Maybe(Maybe&&) = default;
    Maybe& operator=(const Maybe&&) = default;
    Maybe& operator=(T&& newT)
    {
        m_res = newT;
        m_valid = true;
    }

    bool isValid() const
    {
        return m_valid;
    }
    const T& get() const
    {
        assert(m_valid && "Maybe is not valid !");
        return m_res;
    }

private:
    T m_res;
    bool m_valid{};
};

extern QJsonDocument jsonFromFile(const QString& filename);
extern void jsonToFile(const QJsonDocument& document, const QString& filename);

#endif // UTIL_HPP
