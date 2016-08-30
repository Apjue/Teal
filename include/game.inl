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


void Game::initCustomThings()
{
    Ndk::InitializeComponent<Components::DefaultGraphicsPos>("dgpos");
    Ndk::InitializeComponent<Components::Name>("name");
    Ndk::InitializeComponent<Components::Level>("level");
    Ndk::InitializeComponent<Components::Inventory>("inv");
    Ndk::InitializeComponent<Components::CDirection>("cdir");
    Ndk::InitializeComponent<Components::Animation>("anim");
    Ndk::InitializeComponent<Components::Position>("pos");
    Ndk::InitializeComponent<Components::MoveTo>("moveto");
    Ndk::InitializeComponent<Components::Path>("path");
    Ndk::InitializeComponent<Components::Fight>("fight");
    Ndk::InitializeComponent<Components::Life>("life");
    Ndk::InitializeComponent<Components::Map>("map");

    Ndk::InitializeComponent<Items::HPGain>("hpgain");
    Ndk::InitializeComponent<Items::AttackBonus>("atkbonus");
    Ndk::InitializeComponent<Items::AttackResistance>("atkres");
    Ndk::InitializeComponent<Items::Item>("item");
    Ndk::InitializeComponent<Items::Stackable>("stckbl");
    Ndk::InitializeComponent<Items::Equippable>("equip");
    Ndk::InitializeComponent<Items::Edible>("edible");
    Ndk::InitializeComponent<Items::Resource>("res");

    Ndk::InitializeSystem<Systems::AnimationSystem>();
    Ndk::InitializeSystem<Systems::PosRefreshSystem>();
    Ndk::InitializeSystem<Systems::AISystem>();
    Ndk::InitializeSystem<Systems::MovementSystem>();
}

void Game::textureLoadFailed(const Nz::String& file)
{
    Nz::StringStream errLog;
    errLog << __FILE__ << ' ' << __FUNCTION__ << " l" << __LINE__ << ": \n";
    errLog << "Abort. Reason: Texture loading failed - File: " << file;

    NazaraError(errLog);
}

void Game::addTextures()
{
    m_textures.setPrefix("./../../data/img/");

    std::vector<std::pair<Nz::String, Nz::String>> filepaths
    {
        { ":/game/money", "game/main/money.png" },
        { ":/game/scheme", "game/main/scheme.png" },
        { ":/game/teal", "game/main/teal.png" },
        { ":/game/tileset", "game/main/tileset-iso.png" },
        { ":/game/char/villager", "game/char/villager.png" }

        //...
    };

    for (auto& pair : filepaths)
    {
        if (!m_textures.addByLoad(pair.first, pair.second))
            textureLoadFailed(pair.second);
    }
}


void Game::initIcon()
{
    Nz::Image iconImage;
    iconImage.LoadFromFile(m_textures.get(":/game/money")->GetFilePath());

    m_winIcon.Create(iconImage);

    m_window.SetIcon(m_winIcon);
}

void Game::initCam()
{
    m_world->GetSystem<Ndk::RenderSystem>().SetGlobalUp(Nz::Vector3f::Down());

    auto& camera = m_world->CreateEntity();
    camera->AddComponent<Ndk::NodeComponent>();

    auto& cam = camera->AddComponent<Ndk::CameraComponent>();
    cam.SetProjectionType(Nz::ProjectionType_Orthogonal);
}

#endif // GAME_INL
