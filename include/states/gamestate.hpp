// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <NDK/World.hpp>
#include <NDK/Canvas.hpp>
#include <NDK/StateMachine.hpp>
#include <NDK/State.hpp>
#include <NDK/Widgets/ButtonWidget.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Core/Signal.hpp>
#include <Nazara/Renderer/RenderWindow.hpp>
#include <memory>
#include "micropather.h"
#include "data/gamedata.hpp"

class GameState final : public Ndk::State
{
public:
    GameState(GameData& gameData, const Nz::Vector2ui& mapArea);
    ~GameState() override;

    void Enter(Ndk::StateMachine& fsm) override;
    void Leave(Ndk::StateMachine& fsm) override;
    bool Update(Ndk::StateMachine& fsm, float elapsedTime) override;

private:
    Ndk::WorldHandle m_world;
    Nz::RenderWindow& m_window;
    Ndk::EntityHandle m_map;
    Ndk::EntityHandle m_charac; // Main character


    Nz::Rectui m_mapArea;
    std::shared_ptr<micropather::MicroPather> m_pather {}; // Used by the AI System
    std::unique_ptr<Ndk::Canvas> m_canvas;
    Ndk::EntityOwner m_canvasBackgroundEntity;
    bool m_paused { false };


    TilesetCore& m_tilesetCore;
    TilesetCore& m_fightTilesetCore;

    std::shared_ptr<StateMDStore> m_states;
    std::shared_ptr<SkillStore> m_skills;
    AICore& m_ais;
    std::shared_ptr<AnimationStore> m_animations;

    Ndk::EntityList& m_items;
    Ndk::EntityList& m_characters;


    // Raycasting test
    Ndk::EntityHandle m_raycastTiles;


    void handlePausedStateSwitch();

    // Character functions
    void printInventory(bool detailled = false);
    void printCharacteristics();

    // Slots
    NazaraSlot(Nz::EventHandler, OnMouseButtonPressed, m_mouseButtonEvent);
    NazaraSlot(Nz::EventHandler, OnKeyPressed, m_keyPressEvent);
    NazaraSlot(Nz::EventHandler, OnMouseMoved, m_mouseMovedEvent);
    NazaraSlot(Ndk::ButtonWidget, OnButtonTrigger, m_invButtonEvent);

    // Enter
    void enableEntities();
    void addSystems();
    void initEventHandler();
    void addWidgets();

    // Leave
    void removeSystems();
    void disableEntities();
    void removeWidgets();
};

#endif // GAMESTATE_HPP
