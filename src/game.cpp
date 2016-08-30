// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "game.hpp"

Game::Game(Ndk::Application& app, const Nz::Vector2ui& fenSize, 
           const Nz::Vector2ui& viewport, const Nz::String& fenName) :
    m_app(app), m_world(), m_window(app.AddWindow<Nz::RenderWindow>()), m_map(), m_charac()
{
    NazaraUnused(viewport);
    addTextures();

    Nz::ImageRef scheme = Nz::Image::New();
    scheme->LoadFromFile(m_textures.get(":/game/scheme")->GetFilePath());

    setScheme(scheme);

    m_window.Create(Nz::VideoMode(fenSize.x, fenSize.y, 32), fenName);
    m_world = app.AddWorld().CreateHandle();

    initCustomThings();

    initIcon();
    initCam();

    addEntities();
    initEntities();
    addSystems();
    initSystems();
}

void Game::run()
{
    Nz::WindowEvent event;
    while (m_window.PollEvent(&event))
    {
        if (event.type == Nz::WindowEventType_Quit)
            m_app.Quit();
    }

    m_window.Display();
}

void Game::addEntities()
{
    m_map = m_world->CreateEntity();
    m_map->AddComponent<Components::Map>();


    Nz::TextureRef charTex = m_textures.get(":/game/char/villager");

    Nz::SpriteRef charSprite = Nz::Sprite::New( charTex.Get() );
    charSprite->SetTextureRect({ 0u, 0u, 113u, 99u });

    CharacterInfos mainCharacInfos { { 113u, 99u }, charSprite,
                                     15, { -25.f, -66.f }, { 1, 1 }, 100 };
    m_charac = make_character(m_world, mainCharacInfos);
}

void Game::initEntities()
{
    auto& mapComp = m_map->GetComponent<Components::Map>();
    mapComp.map =
    {
            2, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3,
            1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
            1, 1, 1, 2, 1, 2, 2, 2, 3, 2, 3, 3, 3, 3, 3,
            1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
            1, 1, 1, 2, 1, 1, 2, 2, 1, 2, 2, 2, 2, 2, 3,
            0, 0, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2,
            0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2,
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2
    }; //Test
    m_map->Enable(true);
#pragma message("TODO: Use Nz::Tilemap")
    m_pather = std::make_shared<micropather::MicroPather>(&mapComp);

    m_charac->Enable(true);

    m_world->Update();
}

void Game::addSystems()
{
    m_world->AddSystem<Systems::MovementSystem>();
    m_world->AddSystem<Systems::PosRefreshSystem>();
    m_world->AddSystem<Systems::AnimationSystem>();

    m_world->AddSystem<Systems::AISystem>(m_pather);
    m_world->Update();
}

void Game::initSystems()
{
//     m_world->GetSystem<Systems::MapRenderSystem>().updateMap(m_map->GetComponent<Components::Map>());
}
