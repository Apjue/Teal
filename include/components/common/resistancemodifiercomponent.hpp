// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ATTACKRESISTANCECOMPONENT_HPP
#define ATTACKRESISTANCECOMPONENT_HPP

#include <NDK/Component.hpp>

struct ResistanceModifierComponent : public Ndk::Component<ResistanceModifierComponent>
{
    int air {};
    int fire {};
    int water {};
    int earth {};
    int neutral {};

    static Ndk::ComponentIndex componentIndex;
};

#endif // ATTACKRESISTANCECOMPONENT_HPP
