// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ATTACKRESISTANCECOMPONENT_HPP
#define ATTACKRESISTANCECOMPONENT_HPP

#include <NDK/Component.hpp>

struct ResistanceModifierComponent : public Ndk::Component<ResistanceModifierComponent>
{
    ResistanceModifierComponent(int n = 0, int a = 0, int f = 0, int w = 0, int e = 0)
        : neutral { n }, air { a }, fire { f }, water { w }, earth { e } {}

    int neutral {};
    int air {};
    int fire {};
    int water {};
    int earth {};

    static Ndk::ComponentIndex componentIndex;
};

#endif // ATTACKRESISTANCECOMPONENT_HPP
