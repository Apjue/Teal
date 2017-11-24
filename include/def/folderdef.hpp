// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef FOLDERDEF_HPP
#define FOLDERDEF_HPP

#include <Nazara/Core/String.hpp>

namespace Def
{

const Nz::String DataFolder =       "../data/";
const Nz::String ImageFolder = DataFolder +  "img/";
const Nz::String ScriptFolder = DataFolder + "script/";
const Nz::String AnimationFolder = ScriptFolder +    "animations/";
const Nz::String CharacterFolder = ScriptFolder +    "characters/";
const Nz::String MapFolder = ScriptFolder +          "maps/";
const Nz::String SkillFolder = ScriptFolder +        "skills/";
const Nz::String ItemFolder = ScriptFolder +         "items/";
const Nz::String AIFolder = ScriptFolder +           "ais/";
const Nz::String FightAIFolder = AIFolder +               "fight/";
const Nz::String FightAIUtilFile = FightAIFolder +               "util/util.lua";

}

#endif // FOLDERDEF_HPP
