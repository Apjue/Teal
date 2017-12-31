// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ATTACKBONUSCOMPONENT_HPP
#define ATTACKBONUSCOMPONENT_HPP

#include <Nazara/Core/String.hpp>
#include <NDK/Component.hpp>
#include <unordered_map>
#include "data/elementdata.hpp"
#include "def/typedef.hpp"
#include "util/variant.hpp"
#include "util/assert.hpp"
#include "util/util.hpp"

struct AttackModifierComponent : public Ndk::Component<AttackModifierComponent>
{
    AttackModifierComponent(int n = 0, int a = 0, int f = 0, int w = 0, int e = 0)
    {
        data[Element::Neutral] = n;
        data[Element::Air] = a;
        data[Element::Fire] = f;
        data[Element::Water] = w;
        data[Element::Earth] = e;
    }

    AttackModifierComponent(const LuaArguments& args)
    {
        if (args.vars.empty())
            return;

        TealException(args.vars.size() <= 5, "Too much arguments !");

        Element e {};

        for (auto& variant : args.vars)
        {
            data[e] = int(variant.get<double>());
            ++e;

            if (e > Element::Max)
                break;
        }
    }

    ~AttackModifierComponent() = default;

    std::unordered_map<Element, int> data;

    static Ndk::ComponentIndex componentIndex;
};

#endif // ATTACKBONUSCOMPONENT_HPP
