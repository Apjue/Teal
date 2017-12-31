// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LIFECOMPONENT_HPP
#define LIFECOMPONENT_HPP

#include <NDK/Component.hpp>

struct LifeComponent : public Ndk::Component<LifeComponent>
{
    LifeComponent(unsigned maxhp_) : hp { maxhp_ }, maxhp { maxhp_ } {}
    ~LifeComponent() = default;

    unsigned hp { 100 };
    unsigned maxhp { 100 };

    static Ndk::ComponentIndex componentIndex;
};

#endif // LIFECOMPONENT_HPP
