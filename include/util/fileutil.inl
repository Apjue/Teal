// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "fileutil.hpp"

inline Nz::String removeFileNameExtension(Nz::String name)
{
    std::size_t pos = name.FindLast('.');

    if (pos != Nz::String::npos)
        name.Resize(pos);

    return name;
}
