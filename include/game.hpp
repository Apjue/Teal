// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAME_H
#define GAME_H

#include <NDK/World.hpp>
#include <NDK/Components.hpp>
#include <NDK/Application.hpp>
#include <NDK/Components.hpp>
#include <NDK/Systems/RenderSystem.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Graphics/TileMap.hpp>
#include <Nazara/Utility/Icon.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Graphics/ColorBackground.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Utility/EventHandler.hpp>
#include <Nazara/Core/Signal.hpp>
#include <Nazara/Core/File.hpp>
#include <memory>

#include "components.hpp"
#include "systems.hpp"
#include "util/util.hpp"
#include "factory.hpp"
#include "data/characterdata.hpp"
#include "micropather.h"
#include "cache/texturecore.hpp"
#include "cache/mapcore.hpp"
#include "cache/tilesetcore.hpp"
#include "util/maputil.hpp"
#include "util/gfxutil.hpp"

class Game
{
public:
    Game(Ndk::Application& app, const Nz::Vector2ui& winSize, 
         const Nz::Vector2ui& viewport, const Nz::String& fenName);
    ~Game() = default;

    inline void run();

private:
    Ndk::Application& m_app;
    Ndk::WorldHandle m_world;
    Nz::RenderWindow& m_window;

    Ndk::EntityHandle m_map;
    Ndk::EntityHandle m_charac; //main character

    TextureCore m_textures;
    MapCore m_maps;
    TilesetCore m_tilesetCore;

    Nz::Rectui m_mapViewport;
    std::shared_ptr<micropather::MicroPather> m_pather {}; //Used by the AI System
    Nz::Icon m_winIcon;

    //Slots
    NazaraSlot(Nz::EventHandler, OnMouseButtonPressed, m_mouseButtonEvent);

    //Init Functions
    void initCustomThings();

    void addTextures();
    void initTilesetCore();
    void addMaps();
    void textureLoadFailed(const Nz::String& file); // Panic !

    void initIcon();
    void initCam();

    void addEntities();
    void addSystems();

    void initEventHandler();
};

#include "game.inl"

#endif // GAME_H

