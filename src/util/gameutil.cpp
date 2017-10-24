// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/assert.hpp"
#include "util/gameutil.hpp"

namespace
{

Ndk::EntityHandle m_mainCharacter;

}

Ndk::EntityHandle& getMainCharacter()
{
    TealAssert(isGameUtilityInitialized(), "Game utility isn't initialized !");
    return m_mainCharacter;
}

void initializeGameUtility(Ndk::EntityHandle& mainCharacter)
{
    m_mainCharacter = mainCharacter;
}

bool isGameUtilityInitialized()
{
    return m_mainCharacter.IsValid();
}
