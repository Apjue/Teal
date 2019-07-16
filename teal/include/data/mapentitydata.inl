// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/LuaAPI.hpp>
#include "mapentitydata.hpp"

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, MapEntityData* data, TypeTag<MapEntityData>)
{
    state.CheckType(index, Nz::LuaType_Table);

    data->offset = state.CheckField<Nz::Vector2f>("offset", Nz::Vector2f {}, index);
    data->blockTile = state.CheckField<bool>("blocktile", false, index);


    Nz::MaterialRef charMat = Nz::Material::New();
    charMat->Configure("Translucent2D");
    charMat->EnableDepthSorting(true);
    charMat->SetDiffuseMap(Nz::TextureLibrary::Get(state.CheckField<Nz::String>("texture", ":/game/unknown", index)));

    auto size = state.CheckField<Nz::Vector2ui>("size", Nz::Vector2ui(charMat->GetDiffuseMap()->GetSize()), index);

    data->sprite = Nz::Sprite::New(charMat);
    data->sprite->SetMaterial(charMat, false);
    data->sprite->SetTextureRect({ 0u, 0u, size.x, size.y });
    data->sprite->SetSize(float(size.x), float(size.y));

    return 1;
}

} // namespace Nz
