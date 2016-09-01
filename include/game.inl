// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
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

void Game::run()
{
    m_window.Display();
}

#endif // GAME_INL