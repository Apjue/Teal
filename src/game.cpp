// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "game.hpp"

Game::Game(Ndk::Application& app, const Nz::Vector2ui& fenSize, const Nz::String& fenName) :
    m_app(app), m_world(), m_window(app.AddWindow<Nz::RenderWindow>()), m_map(), m_charac()
{
    m_window.Create(Nz::VideoMode(fenSize.x, fenSize.y, 32), fenName);

    Nz::Image iconImage;
    iconImage.LoadFromFile(":/game/money");
#error Change filepath
    m_winIcon.Create(iconImage);

    m_window.SetIcon(m_winIcon);

    m_world = app.AddWorld().CreateHandle();
    m_world->GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());

    auto& camera = m_world->CreateEntity();
    camera->AddComponent<Ndk::NodeComponent>();

    auto& cam = camera->AddComponent<Ndk::CameraComponent>();
    cam.SetProjectionType(Nz::ProjectionType_Orthogonal);

    addEntities();
    initEntities();
    addSystems();
    initSystems();
}

void Game::update()
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

    Nz::TextureRef charTex = Nz::Texture::New();
    charTex->LoadFromFile(":/game/char/villager");
#error use new filepath

    CharacterInfos mainCharacInfos { {113, 99}, Nz::Sprite::New( charTex ),
                           15, {-25, -66}, {1, 1}, 100 };
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
#error Use Nz::Tilemap
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
