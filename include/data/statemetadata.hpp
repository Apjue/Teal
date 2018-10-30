// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef STATEMETADATA_HPP
#define STATEMETADATA_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Renderer/Texture.hpp>

struct StateMetaData
{
    Nz::String codename;
    Nz::String name;
    Nz::String description;
    Nz::TextureRef icon;
};

#include <Nazara/Lua/LuaState.hpp>

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, StateMetaData* data, TypeTag<StateMetaData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    data->codename = state.CheckField<Nz::String>("codename", index);
    data->name = state.CheckField<Nz::String>("name", index);
    data->description = state.CheckField<Nz::String>("description", index);

    Nz::String icon = state.CheckField<Nz::String>("icon", index);
    data->icon = (Nz::TextureLibrary::Has(icon) ? Nz::TextureLibrary::Get(icon) : Nz::TextureLibrary::Get(":/game/unknown"));

    return 1;
}

} // namespace Nz

#endif // STATEMETADATA_HPP
