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
}

void Game::addTextures()
{
    m_textures.setPrefix("./../../data/img/");

    std::vector<std::pair<Nz::String, Nz::String>> filepaths
    {
        { ":/game/money", "game/main/money.png" },
        { ":/game/scheme", "game/main/scheme.png" },
        { ":/game/teal", "game/main/teal.png" },
        { ":/game/tileset", "game/main/tileset.png" },
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
    Ndk::InitializeComponent<RandomMovementComponent>("rdmov");
    Ndk::InitializeComponent<DefaultGraphicsPosComponent>("dgpos");
    Ndk::InitializeComponent<NameComponent>("name");
    Ndk::InitializeComponent<LevelComponent>("level");
    Ndk::InitializeComponent<InventoryComponent>("inv");
    Ndk::InitializeComponent<OrientationComponent>("orien");
    Ndk::InitializeComponent<AnimationComponent>("anim");
    Ndk::InitializeComponent<PositionComponent>("pos");
    Ndk::InitializeComponent<MoveToComponent>("moveto");
    Ndk::InitializeComponent<PathComponent>("path");
    Ndk::InitializeComponent<FightComponent>("fight");
    Ndk::InitializeComponent<LifeComponent>("life");
    Ndk::InitializeComponent<MapComponent>("map");

    Ndk::InitializeComponent<Items::HPGainComponent>("hpgain");
    Ndk::InitializeComponent<Items::AttackBonusComponent>("atkbonus");
    Ndk::InitializeComponent<Items::AttackResistanceComponent>("atkres");
    Ndk::InitializeComponent<Items::ItemComponent>("item");
    Ndk::InitializeComponent<Items::EquippableComponent>("equip");
    Ndk::InitializeComponent<Items::EdibleComponent>("edible");
    Ndk::InitializeComponent<Items::ResourceComponent>("res");

    //Systems
    Ndk::InitializeSystem<AnimationSystem>();
    Ndk::InitializeSystem<PosRefreshSystem>();
    Ndk::InitializeSystem<AISystem>();
    Ndk::InitializeSystem<MovementSystem>();
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
    rendersys.SetDefaultBackground(Nz::ColorBackground::New(Nz::Color::White));
}

void Game::addEntities()
{
    MapData mapData
    {
        {
            2, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3,
            1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
            1, 1, 1, 2, 1, 2, 2, 2, 3, 2, 3, 3, 3, 3, 3,
            1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
            1, 1, 1, 2, 1, 1, 2, 2, 1, 2, 2, 2, 2, 2, 3,
            0, 0, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2,
            0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2,
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2
        }
    };

    m_map = m_world->CreateEntity();

    auto& mapComp = m_map->AddComponent<MapComponent>();

    m_pather = std::make_shared<micropather::MicroPather>(mapComp.map.get());


    m_map->AddComponent<Ndk::GraphicsComponent>();
    m_map->AddComponent<Ndk::NodeComponent>();


    Nz::MaterialRef charMat = Nz::Material::New();
    charMat->LoadFromFile( m_textures.get(":/game/char/villager")->GetFilePath() );

    charMat->EnableBlending(true);
    charMat->SetDstBlend(Nz::BlendFunc_InvSrcAlpha);
    charMat->SetSrcBlend(Nz::BlendFunc_SrcAlpha);
    charMat->EnableDepthWrite(false);

    Nz::SpriteRef charSprite = Nz::Sprite::New(charMat);
    charSprite->SetTextureRect({ 0u, 0u, 113u, 99u });

    CharacterData mainCharacData { { 113u, 99u }, charSprite,
                                     15, { -25.f, -66.f }, { 1, 1 }, 100 };
    m_charac = make_character(m_world, mainCharacData);
}

void Game::initEntities()
{
}

void Game::addSystems()
{
    m_world->AddSystem<MovementSystem>();
    m_world->AddSystem<PosRefreshSystem>();
    m_world->AddSystem<AnimationSystem>();
    m_world->AddSystem<AISystem>(m_pather);
    // m_world->AddSystem<MapSystem>();
}

void Game::initSystems()
{
    //...
}

void Game::initEventHandler()
{
    auto& eventHandler = m_window.GetEventHandler();

    m_mouseButtonEvent.Connect(eventHandler.OnMouseButtonPressed,
    [this] (const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent& event)
    { // Function to move the player if the user clicked in the map
        if (m_mapViewport.Contains(event.x, event.y))
        {
            auto& movecomp = m_charac->GetComponent<MoveToComponent>();
            auto lpos = getTileFromGlobalCoords({ event.x, event.y });

            movecomp.diffX = lpos.x;
            movecomp.diffY = lpos.y;
        }
    });
}

