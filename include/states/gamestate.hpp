// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
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
    GameState(Ndk::WorldHandle world, Nz::RenderWindow& window, const Nz::Vector2ui& mapArea, GameData& gameData);
    ~GameState() override = default;

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
    bool m_paused { false };

    TilesetCore& m_tilesetCore;
    TilesetCore& m_fightTilesetCore;

    StateMDStore& m_states;
    EffectMDStore& m_effects;
    SkillStore& m_skills;
    AICore& m_ais;
    AnimationStore& m_animations;

    Ndk::EntityList& m_items;
    Ndk::EntityList& m_characters;

    void handlePausedState();

    // Character functions
    void showInventory(bool detailled = false);
    void showCharacteristics();

    // Slots
    NazaraSlot(Nz::EventHandler, OnMouseButtonPressed, m_mouseButtonEvent);
    NazaraSlot(Nz::EventHandler, OnKeyPressed, m_keyPressEvent);
    NazaraSlot(Nz::EventHandler, OnMouseMoved, m_mouseMovedEvent);
    NazaraSlot(Ndk::ButtonWidget, OnButtonTrigger, m_invButtonEvent);

    void addEntities();
    void addSystems();

    void initEventHandler();
    void addWidgets();

    // And now, delete...
    void removeSystems();
    void killEntities();
    void uninitEventHandler(); // change this name
};

#endif // GAMESTATE_HPP
