// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef UTIL_HPP
#define UTIL_HPP

#include <assert.h>
#include <array>

///
/// \brief The non_nullptr class
///
/// This class ensure that its pointer is never nullptr
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

class Def
{
public:
    static const unsigned MAPX           {15u}; //For arrays.
    static const unsigned MAPY           {8u};

    static const unsigned TILEARRAYSIZE  {MAPX*MAPY};
    using  TILEARRAY    = std::array     <unsigned, TILEARRAYSIZE>;

    static const unsigned MAPXMAP        {14u}; //For logic
    static const unsigned MAPYMAP        {14u};

    static const unsigned MAPXSIZE       {512u};
    static const unsigned MAPYSIZE       {256u};

    static const unsigned BUTTONSXSIZE   {MAPXSIZE};
    static const unsigned BUTTONSYSIZE   {90u};

    static const unsigned TILEXSIZE      {64u};
    static const unsigned TILEYSIZE      {32u};

    static const unsigned TILEGXSIZE     {TILEXSIZE / 2};
    static const unsigned TILEGYSIZE     {TILEYSIZE / 2};

    static const unsigned MAXPOSINTILE   {4u};
    static const unsigned MAXGXPOSINTILE {TILEGXSIZE / MAXPOSINTILE};
    static const unsigned MAXGYPOSINTILE {TILEGYSIZE / MAXPOSINTILE};
};

inline unsigned dToU(double d)
{
    return static_cast<unsigned>(d);
}

template<class T>
T distance(const T& x, const T& y)
{
    return (x > y) ? x - y : y - x;
}

using AbsTile = std::pair<unsigned, unsigned>; // Absolute Tile (0-15, 0-8)
using DiffTile = std::pair<int, int>; //Difference Tile (can be negative or positive)

#endif // UTIL_HPP
