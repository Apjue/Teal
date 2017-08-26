// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

inline LuaArguments parseLua(Nz::LuaInstance& lua)
{
    LuaArguments args;
    parseLua(lua, args);

    return args;
}
