// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/assert.hpp"
#include "util/luaparser.hpp"

void parseLua(Nz::LuaInstance& lua, LuaArguments& table)
{
    for (unsigned i { 1 };; ++i)
    {
        lua.PushInteger(i);

        if (lua.GetTable() == Nz::LuaType_Nil)
        {
            lua.Pop();
            break;
        }

        switch (lua.GetType(-1))
        {
            case Nz::LuaType_Boolean:
            {
                LuaBasicArgument arg;
                arg.set<bool>(lua.CheckBoolean(-1));

                table.vars.push_back(arg);
                break;
            }

            case Nz::LuaType_Number:
            {
                LuaBasicArgument arg;
                arg.set<double>(lua.CheckNumber(-1));

                table.vars.push_back(arg);
                break;
            }

            case Nz::LuaType_String:
            {
                LuaBasicArgument arg;
                arg.set<Nz::String>(Nz::String { lua.CheckString(-1) });

                table.vars.push_back(arg);
                break;
            }

            case Nz::LuaType_Table:
            {
                LuaTableArgument tableArg;
                table.tables.push_back(std::make_shared<LuaTableArgument>(tableArg));

                parseLua(lua, *(table.tables.back().get()));
                break;
            }

            default:
                NazaraError("Lua parser: Type not supported");
                break;
        }

        lua.Pop();
    }
}
