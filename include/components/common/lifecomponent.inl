// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

bool LifeComponent::alive() const noexcept
{
    return m_hp == 0;
}

unsigned LifeComponent::HP() const noexcept
{
    return m_hp;
}

void LifeComponent::setHP(unsigned hp)
{
    m_hp = hp;
    verifyInfos();
}

unsigned LifeComponent::maxHP() const noexcept
{
    return m_maxhp;
}

void LifeComponent::setMaxHP(unsigned maxHP)
{
    m_maxhp = maxHP;
    verifyInfos();
}

void LifeComponent::verifyInfos()
{
    m_hp = (m_hp > m_maxhp) ? m_maxhp : m_hp;
}
