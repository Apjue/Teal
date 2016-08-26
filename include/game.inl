// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAME_INL
#define GAME_INL

Ndk::WorldHandle Game::getWorld() const
{
    return m_world->CreateHandle();
}
Ndk::EntityHandle Game::getPerso()
{
    return m_charac->CreateHandle();
}
Ndk::EntityHandle Game::getMap()
{
    return m_map->CreateHandle();
}

#endif // GAME_INL
