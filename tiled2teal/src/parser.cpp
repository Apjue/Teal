// Copyright (C) 2017 Samy Bensaid
// This file is part of the Tiled2Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Nazara/Lua.hpp>
#include <Nazara/Core/Log.hpp>
#include <Nazara/Core/File.hpp>
#include <array>
#include <stdexcept>
#include <sstream>
#include "data/tiledata.hpp"
#include "util/assert.hpp"
#include "containers/tilesetcore.hpp"
#include "parser.hpp"

namespace
{

TilesetCore m_tilesetCore;
TilesetCore m_fightTilesetCore;

}

void initTilesetCore()
{
    Nz::String scriptPrefix = "../data/script/";
    TealException(Nz::File::Exists(scriptPrefix + "tilesetcore.lua"), "tilesetcore.lua not found !");

    Nz::LuaInstance lua;
    TealException(lua.ExecuteFromFile(scriptPrefix + "tilesetcore.lua"), "Lua: tilesetcore.lua loading failed !");

    {
        TealException(lua.GetGlobal("teal_tileset_core") == Nz::LuaType_Table, "Lua: teal_tileset_core isn't a table !");

        for (int i { 1 };; ++i)
        {
            lua.PushInteger(i);

            if (lua.GetTable() != Nz::LuaType_Table)
            {
                lua.Pop();
                break;
            }

            m_tilesetCore.add(lua.CheckField<unsigned>("index"), lua.CheckField<Nz::String>("name"));
            lua.Pop();
        }

        lua.Pop();
    }

    {
        TealException(lua.GetGlobal("teal_fight_tileset_core") == Nz::LuaType_Table, "Lua: teal_fight_tileset_core isn't a table !");

        m_fightTilesetCore.add(lua.CheckField<unsigned>("walk"), "walk");
        m_fightTilesetCore.add(lua.CheckField<unsigned>("empty"), "empty");
        m_fightTilesetCore.add(lua.CheckField<unsigned>("red"), "red");
        m_fightTilesetCore.add(lua.CheckField<unsigned>("blue"), "blue");

        lua.Pop();
    }
}

void convertTiledMap(const Nz::String& tex, const Nz::String& flags, const Nz::String& out, const Nz::String& pos)
{
    constexpr unsigned tileArraySize = 120u;
    std::array<TileData, tileArraySize> map;

    {
        auto array = parseCsv(tex);

        for (unsigned i {}; i < array.size(); ++i)
            if (array[i] != -1)
                map[i].textureId = array[i];
    }

    {
        auto array = parseCsv(flags);

        for (unsigned i {}; i < array.size(); ++i)
        {
            if (array[i] != -1)
            {
                Nz::String value = m_fightTilesetCore.get(array[i]);
                TileFlag flag = TileFlag::NoFlag;

                if (value == "empty")
                    flag = TileFlag::ViewObstacle;

                if (value == "red")
                    flag = TileFlag::RedSpawn;

                if (value == "blue")
                    flag = TileFlag::BlueSpawn;

                map[i].flag = flag;
            }

            else
                map[i].flag = TileFlag::Invisible;
        }

    }
    
    Nz::File tealMap { out, Nz::OpenMode_WriteOnly | Nz::OpenMode_Truncate | Nz::OpenMode_Text };

    tealMap.Write("teal_map =\n{\n    pos = \"");
    tealMap.Write(pos);
    tealMap.Write("\",\n    entities = {},\n\n");

    for (const TileData& tile : map)
    {
        tealMap.Write("    { texId = \"");
        tealMap.Write(tile.flag != TileFlag::Invisible ? m_tilesetCore.get(tile.textureId) : "");

        tealMap.Write("\", flag = \"");
        tealMap.Write(tileFlagToString(tile.flag));

        tealMap.Write("\" },\n");
    }

    tealMap.Write("}");
}

std::vector<int> parseCsv(const Nz::String& filepath)
{
    Nz::File file { filepath, Nz::OpenMode_MustExist | Nz::OpenMode_ReadOnly | Nz::OpenMode_Text };

    if (!file.IsOpen())
        throw std::runtime_error { Nz::String { "Failed to open file with path " }.Append(filepath).ToStdString() };

    Nz::String filestr;
    filestr.Resize(file.GetSize());

    file.Read(filestr.GetBuffer(), static_cast<size_t>(file.GetSize()));
    std::istringstream s { filestr.GetUtf8String() };

    std::string result;

    for (std::string temp;;)
    {
        bool stop {};

        if (!(std::getline(s, temp)))
            stop = true;

        temp.append(",");
        Nz::String tempNz = temp;
        tempNz.ReplaceAny("\r\n\t", '\0');

        std::vector<int> numbersTemp;
        std::vector<Nz::String> splitsTemp;

        tempNz.Split(splitsTemp, ",");

        for (unsigned i {}; i < splitsTemp.size(); ++i)
        {
            long long integer;
            splitsTemp[i].ToInteger(&integer);
            numbersTemp.push_back(static_cast<int>(integer));
        }

        std::stable_partition(numbersTemp.begin(), numbersTemp.end(), [&numbersTemp] (const int& a) -> bool
        {
            return (&a - numbersTemp.data()) % 2 == 0;
        });

        std::string final;

        for (int& i : numbersTemp)
            final.append(std::to_string(i)).append(",");

        result += final;

        if (stop)
        {
            result.erase(result.end() - 1);
            break;
        }
    }

    std::vector<int> numbers;
    std::vector<Nz::String> splits;

    filestr = result;
    filestr.Split(splits, ",");

    for (unsigned i {}; i < splits.size(); ++i)
    {
        long long integer;
        splits[i].ToInteger(&integer);
        numbers.push_back(static_cast<int>(integer));
    }

    return numbers;
}
