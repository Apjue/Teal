// Copyright (C) 2018 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

// Nz::Vector2 and std::pair

template<class T>
std::pair<T, T> toPair(const Nz::Vector2<T>& v)
{
    return std::make_pair<T, T>(v.x, v.y);
}

template<class T>
Nz::Vector2<T> toVector2(const std::pair<T, T>& v)
{
    return { v.first, v.second };
}
