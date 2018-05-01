// Copyright (C) 2018 Samy Bensaid
// This file is part of the  project.
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
    template<typename T>
    unsigned int LuaImplQueryArg(const LuaState& instance, int index, std::vector<T>* container, TypeTag<std::vector<T>>)
    {
        instance.CheckType(index, Nz::LuaType_Table);
        std::size_t pos = 1;

        for (;;)
        {
            instance.PushInteger(pos++);

            if (instance.GetTable() == Nz::LuaType_Nil)
            {
                instance.Pop();
                break;
            }

            T arg {};

            if (LuaImplQueryArg(instance, -1, &arg, TypeTag<T>()) != 1)
            {
                instance.Error("Type needs more than one place to be initialized");
                return 0;
            }

            container->push_back(arg);
            instance.Pop();
        }

        return 1;
    }
}

