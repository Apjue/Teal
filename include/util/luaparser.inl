// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

inline LuaArguments parseLua(Nz::LuaInstance& lua)
{
    LuaArguments args;
    parseLua(lua, args);

    return args;
}

inline bool hasField(Nz::LuaInstance& lua, const Nz::String& fieldName, int tableIndex)
{
    bool exists = (lua.GetField(fieldName, tableIndex) != Nz::LuaType_Nil);
    lua.Pop();

    return exists;
}
