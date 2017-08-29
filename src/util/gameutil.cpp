// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/gameutil.hpp"

namespace
{

Ndk::EntityHandle m_mainCharacter;

}

Ndk::EntityHandle& getMainCharacter()
{
    return m_mainCharacter;
}

void initializeGameUtility(Ndk::EntityHandle& mainCharacter)
{
    m_mainCharacter = mainCharacter;
}

bool isGameUtilityInitialized()
{
    return m_mainCharacter.IsValid() && m_mainCharacter->IsValid();
}
