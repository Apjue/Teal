// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LIFECOMPONENT_HPP
#define LIFECOMPONENT_HPP

#include <NDK/Component.hpp>

class LifeComponent : public Ndk::Component<LifeComponent>
{
public:
    LifeComponent(unsigned maxhp) : m_hp { maxhp }, m_maxhp { maxhp } {}
    ~LifeComponent() = default;

    inline bool alive() const noexcept;
    inline unsigned HP() const noexcept;
    inline void setHP(unsigned hp);

    inline unsigned maxHP() const noexcept;
    inline void setMaxHP(unsigned maxHP);

    static Ndk::ComponentIndex componentIndex;

private:
    unsigned m_hp { 100 };
    unsigned m_maxhp { 100 };

    inline void verifyInfos();
};

#include "lifecomponent.inl"

#endif // LIFECOMPONENT_HPP
