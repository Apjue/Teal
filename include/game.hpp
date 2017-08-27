// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAME_HPP
#define GAME_HPP

#include <NDK/Application.hpp>
#include <NDK/World.hpp>
#include <NDK/EntityList.hpp>
#include <NDK/Canvas.hpp>
#include <NDK/Widgets/ButtonWidget.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <Nazara/Utility/Icon.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Utility/EventHandler.hpp>
#include <Nazara/Core/Signal.hpp>
#include <memory>
#include "micropather.h"
#include "cache/tilesetcore.hpp"
#include "cache/doublestore.hpp"
#include "util/variant.hpp"

class Game
{
public:
    Game(Ndk::Application& app, const Nz::Vector2ui& winSize,
         const Nz::Vector2ui& viewport, const Nz::String& winName);
    ~Game() = default;

    inline void run();

private:
    Ndk::Application& m_app;
    Ndk::WorldHandle m_world;
    Nz::RenderWindow& m_window;

    Ndk::EntityHandle m_map;
    Ndk::EntityHandle m_charac; // Main character

    TilesetCore m_tilesetCore;
    TilesetCore m_fightTilesetCore;

    Nz::Rectui m_mapViewport;
    std::shared_ptr<micropather::MicroPather> m_pather {}; // Used by the AI System
    Nz::IconRef m_winIcon;
    std::unique_ptr<Ndk::Canvas> m_canvas; // Wait until Nazara fix widgets bugs to use this more (Nazara 0.4.0 ?)

    bool m_paused { false };
    Ndk::EntityHandle m_background;
    Ndk::EntityHandle m_pauseText;

    const Nz::String m_rootPrefix =         "../data/";
    const Nz::String m_imgPrefix = m_rootPrefix +    "img/";
    const Nz::String m_addonsPrefix = m_rootPrefix + "addons/";
    const Nz::String m_addonsImgPrefix = m_addonsPrefix +    "img/";
    const Nz::String m_scriptPrefix = m_rootPrefix + "script/";
    
    StateMDStore m_states;
    EffectMDStore m_effects;
    SkillStore m_skills;

    Ndk::EntityList m_items;
    Ndk::EntityList m_characters;

    // Illegal functions
    Ndk::EntityHandle cloneCharacter(const Nz::String& codename);

    // Character functions
    void showInventory(bool detail = false);
    void showCharacteristics();

    // Slots
    NazaraSlot(Nz::EventHandler, OnMouseButtonPressed, m_mouseButtonEvent);
    NazaraSlot(Nz::EventHandler, OnKeyPressed, m_keyPressEvent);
    NazaraSlot(Nz::EventHandler, OnMouseMoved, m_mouseMovedEvent);
    NazaraSlot(Ndk::ButtonWidget, OnButtonTrigger, m_invButtonEvent);

    // Init Functions
    void initNazara();

    void loadTextures();
    void loadTilesetCore();

    void loadCharacters();
    //void loadMapObjects();
    void loadMaps();

    void loadMetaData();
    void loadSkills();
    void loadItems();

    void addIcon();
    void addCam();

    void addEntities();
    void addSystems();

    void initEventHandler();
    void addWidgets();

    void addPauseMenu();
    void enablePauseMenu(bool enable = true);
};

#include "game.inl"

#endif // GAME_HPP
