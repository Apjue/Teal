// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef QTDEBUGUTIL_HPP
#define QTDEBUGUTIL_HPP

#include <QDebug>
#include <unordered_map>
#include <utility>

template<class T1, class T2>
QDebug& operator<<(QDebug& output, const std::pair<T1, T2>& input)
{
    output << "[" << input.first << ", " << input.second << "]";
    return output;
}

template<class Key, class T, class Hash, class KeyEqual, class Allocator>
QDebug& operator<<(QDebug& output, const std::unordered_map<Key, T, Hash, KeyEqual, Allocator>& input)
{
    for (auto&& item: input)
    {
        output << item;
    }
    return output;
}

#endif // QTDEBUGUTIL_HPP
