// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/luaparser.hpp"

LuaArguments parseLua(Nz::LuaInstance & lua)
{
    std::map<int /*depth*/, int /*index*/> index;
    LuaArguments arguments;
    arguments.push_back({});

    for (unsigned j { 1 }, depth { 1 }, tableIndex { 1 };; ++j)
    {
        lua.PushInteger(j);

        if (lua.GetTable() == Nz::LuaType_Nil)
        {
            lua.Pop(1);
            --depth;
            ++tableIndex;

            if (depth == 0)
                break;

            j = index[depth];
            index[depth] = 0;

            continue;
        }

        LuaArgument table;
        LuaBasicArgument arg;

        switch (lua.GetType(-1))
        {
            case Nz::LuaType_Boolean:
                arg.set<bool>(lua.CheckBoolean(-1));
                (arguments.end() - tableIndex)->args.push_back(arg);

                break;

            case Nz::LuaType_Number:
                arg.set<double>(lua.CheckNumber(-1));
                (arguments.end() - tableIndex)->args.push_back(arg);

                break;

            case Nz::LuaType_String:
                arg.set<Nz::String>(Nz::String { lua.CheckString(-1) });
                (arguments.end() - tableIndex)->args.push_back(arg);

                break;

            case Nz::LuaType_Table:
                index[depth] = j;
                j = 0;

                tableIndex = 1;
                arguments.push_back(table);

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
