// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAME_H
#define GAME_H

#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Utility/Icon.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <NDK/World.hpp>
#include <NDK/Application.hpp>
#include <Nazara/Core/Error.hpp>
#include <memory>

#include "components.hpp"
#include "systems.hpp"
#include "util.hpp"
#include "factory.hpp"
#include "chrono.hpp"
#include "characterinfos.hpp"
#include "micropather.h"
#include "texturecore.hpp"

class Game
{
public:
    Game(Ndk::Application& app, const Nz::Vector2ui& fenSize, 
         const Nz::Vector2ui& viewport, const Nz::String& fenName);
    ~Game() = default;

    inline Ndk::WorldHandle getWorld() const;
    inline Ndk::EntityHandle getPerso();
    inline Ndk::EntityHandle getMap();

    void run();

private:
    Ndk::Application& m_app;
    Ndk::WorldHandle m_world;
    Nz::RenderWindow& m_window;
    Ndk::EntityHandle m_map;
    Ndk::EntityHandle m_charac; //main character
    TextureCore m_textures;

    unsigned m_fpsCounter {};
    unsigned m_fps {};

    std::shared_ptr<micropather::MicroPather> m_pather {}; //Used by the AI System
    Nz::Icon m_winIcon;

    inline void initCustomThings();
    inline void textureLoadFailed(const Nz::String& file); // Panic !

    inline void addTextures();
    inline void initIcon();
    inline void initCam();

    void addEntities();
    void initEntities();
    void addSystems();
    void initSystems();
};

#include "game.inl"

#endif // GAME_H
