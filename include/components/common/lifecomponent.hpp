// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LIFECOMPONENT_HPP
#define LIFECOMPONENT_HPP

#include <NDK/Component.hpp>

struct LifeComponent : public Ndk::Component<LifeComponent>
{
    LifeComponent(unsigned maxhp) : m_hp { maxhp }, m_maxhp { maxhp } {}
    ~LifeComponent() = default;

    unsigned m_hp { 100 };
    unsigned m_maxhp { 100 };
};

#endif // LIFECOMPONENT_HPP
