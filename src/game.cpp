// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "game.hpp"

Game::Game(Ndk::Application& app, const Nz::Vector2ui& winSize, 
           const Nz::Vector2ui& viewport, const Nz::String& fenName) :
    m_app(app), m_world(), m_window(app.AddWindow<Nz::RenderWindow>()), m_map(), m_charac(),
    m_mapViewport(viewport)
{
    addTextures();

    Nz::ImageRef scheme = Nz::Image::New();
    scheme->LoadFromFile(m_textures.get(":/game/scheme")->GetFilePath());

    setScheme(scheme);

    m_window.Create(Nz::VideoMode(winSize.x, winSize.y, 32), fenName);
    m_world = app.AddWorld().CreateHandle();

    initCustomThings();

    initIcon();
    initCam();

    addEntities();
    initEntities();
    addSystems();
    initSystems();
}


void Game::textureLoadFailed(const Nz::String& file)
{
    Nz::StringStream errLog;
    errLog << __FILE__ << ' ' << __FUNCTION__ << " l" << __LINE__ << ": \n";
    errLog << "Abort. Reason: Texture loading failed - File: " << file;

    NazaraError(errLog);
    NazaraAssert(false, "Texture Loading failed ! See previous error message");
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

void Game::initCustomThings()
{
    Ndk::InitializeComponent<Components::RandomMovement>("rdmov");
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

    //Systems
    Ndk::InitializeSystem<Systems::AnimationSystem>();
    Ndk::InitializeSystem<Systems::PosRefreshSystem>();
    Ndk::InitializeSystem<Systems::AISystem>();
    Ndk::InitializeSystem<Systems::MovementSystem>();
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
    auto& camera = m_world->CreateEntity();
    camera->AddComponent<Ndk::NodeComponent>();

    auto& cam = camera->AddComponent<Ndk::CameraComponent>();
    cam.SetProjectionType(Nz::ProjectionType_Orthogonal);

    cam.SetTarget(&m_window);

    auto& rendersys = m_world->GetSystem<Ndk::RenderSystem>();

    rendersys.SetGlobalUp(Nz::Vector3f::Down());
    rendersys.SetDefaultBackground(Nz::ColorBackground::New(Nz::Color(Nz::Color(255, 20, 147))));
}

void Game::addEntities()
{
    m_map = m_world->CreateEntity();
    m_map->AddComponent<Components::Map>();


    Nz::MaterialRef charMat = Nz::Material::New();
    charMat->LoadFromFile( m_textures.get(":/game/char/villager")->GetFilePath() );

    Nz::TextureRef charTex = Nz::Texture::New();
    charTex->LoadFromFile( m_textures.get(":/game/char/villager")->GetFilePath() );

//     charMat->EnableBlending(true);
//     charMat->SetDstBlend(Nz::BlendFunc_InvSrcAlpha);
//     charMat->SetSrcBlend(Nz::BlendFunc_SrcAlpha);
//     charMat->EnableDepthWrite(false);

    Nz::SpriteRef charSprite = Nz::Sprite::New(charTex.Get());
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
}

void Game::addSystems()
{
//     m_world->AddSystem<Systems::MovementSystem>();
//     m_world->AddSystem<Systems::PosRefreshSystem>();
//     m_world->AddSystem<Systems::AnimationSystem>();
// 
//     m_world->AddSystem<Systems::AISystem>(m_pather);
}

void Game::initSystems()
{
//     m_world->GetSystem<Systems::MapSystem>().updateMap(m_map->GetComponent<Components::Map>());

    m_world->Update();
}

void Game::initEventHandler()
{
    auto& eventHandler = m_window.GetEventHandler();

    m_mouseButtonEvent.Connect(eventHandler.OnMouseButtonPressed,
    [this] (const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent& event)
    { // Function to move the player if the user clicked in the map
        if (m_mapViewport.Contains(event.x, event.y))
        {
            auto& movecomp = m_charac->GetComponent<Components::MoveTo>();
            auto lpos = getTileFromGlobalCoords({ event.x, event.y });

            movecomp.diffX = lpos.x;
            movecomp.diffY = lpos.y;
        }
    });
}
