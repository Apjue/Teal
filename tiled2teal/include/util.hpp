// Copyright (C) 2017 Samy Bensaid
// This file is part of the Tiled2Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <limits>

template<class T>
bool safeAnswer(std::istream& in, T& t)
{
    if (!(in >> t))
    {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }

    return true;
}

#endif // UTIL_HPP
