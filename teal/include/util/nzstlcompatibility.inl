// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

// Nz::Vector2 and std::pair

template<class T>
std::pair<T, T> toPair(const Nz::Vector2<T>& v)
{
    return std::make_pair<T, T>(v.x, v.y);
}

template<class T>
Nz::Vector2<T> toVector2(const std::pair<T, T>& v)
{
    return { v.first, v.second };
}

namespace Nz
{

template<class T1, class T2>
unsigned LuaImplQueryArg(const LuaState& state, int index, std::pair<T1, T2>* pair, TypeTag<std::pair<T1, T2>>)
{
    state.CheckType(index, Nz::LuaType_Table);

    {
        state.PushInteger(1);
        state.GetTable();

        int index_ { -1 };
        pair->first = state.Check<T1>(&index_);

        state.Pop();
    }

    {
        state.PushInteger(2);
        state.GetTable();

        int index_ { -1 };
        pair->second = state.Check<T2>(&index_);

        state.Pop();
    }

    return 1;
}


template<class T1, class T2>
unsigned LuaImplQueryArg(const LuaState& state, int index, std::unordered_map<T1, T2>* map, TypeTag<std::unordered_map<T1, T2>>)
{
    state.CheckType(index, Nz::LuaType_Table);

    for (int i { 1 };; ++i)
    {
        state.PushInteger(i);

        if (state.GetTable() == Nz::LuaType_Table)
        {
            std::pair<T1, T2> pair;
            LuaImplQueryArg(state, -1, &pair, TypeTag<std::pair<T1, T2>>());

            map->insert(pair);
            state.Pop();
        }

        else
        {
            state.Pop();
            break;
        }
    }

    return 1;
}

template<class T1, class T2>
unsigned LuaImplQueryArg(const LuaState& state, int index, std::map<T1, T2>* map, TypeTag<std::map<T1, T2>>)
{
    state.CheckType(index, Nz::LuaType_Table);

    for (int i { 1 };; ++i)
    {
        state.PushInteger(i);

        if (state.GetTable() == Nz::LuaType_Table)
        {
            std::pair<T1, T2> pair;
            LuaImplQueryArg(state, -1, &pair, TypeTag<std::pair<T1, T2>>());

            map->insert(pair);
            state.Pop();
        }

        else
        {
            state.Pop();
            break;
        }
    }

    return 1;
}

} // namespace Nz
