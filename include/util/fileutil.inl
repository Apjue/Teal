// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "fileutil.hpp"

inline Nz::String& removeFileNameExtension(Nz::String& name)
{
    std::size_t pos = name.FindLast('.');

    if (pos != Nz::String::npos)
        name.Resize(pos);

    return name;
}
