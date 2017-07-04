// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/luaparser.hpp"

LuaArguments parseLua(Nz::LuaInstance& lua)
{
    std::map<int /*depth*/, int /*index*/> index;
    LuaArguments arguments;

    for (unsigned i { 1 }, depth { 1 }, tableIndex { 1 }; depth > 0; ++i)
    {
        lua.PushInteger(i);

        if (lua.GetTable() == Nz::LuaType_Nil)
        {
            lua.Pop();
            --depth;
            ++tableIndex;

            i = index[depth];
            index[depth] = 1;

            continue;
        }

        LuaTableArgument table; // BUG: tableindex
        LuaBasicArgument arg;

        switch (lua.GetType(-1))
        {
            case Nz::LuaType_Boolean:
                arg.set<bool>(lua.CheckBoolean(-1));
                arguments.vars.push_back(arg);

                break;

            case Nz::LuaType_Number:
                arg.set<double>(lua.CheckNumber(-1));
                arguments.vars.push_back(arg);

                break;

            case Nz::LuaType_String:
                arg.set<Nz::String>(Nz::String { lua.CheckString(-1) });
                arguments.vars.push_back(arg);

                break;

            case Nz::LuaType_Table:
                index[depth] = i;
                i = 1;

                tableIndex = 1;
                arguments.tables.push_back(std::make_shared<LuaTableArgument>(table));

                ++depth;
                break;

            default:
                NazaraError("Lua parser: Type not supported");
                break;
        }

        lua.Pop();
    }

    return arguments;
}
