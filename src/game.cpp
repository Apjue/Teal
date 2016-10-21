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

    initSchemeUtility(scheme);

    m_window.Create(Nz::VideoMode(winSize.x, winSize.y, 32), fenName);
    m_world = app.AddWorld().CreateHandle();

    initCustomThings();

    initTilesetCore();
    addMaps();

    initIcon();
    initCam();

    addEntities();
    addSystems();
    initEventHandler();

    auto& mapComp = m_map->GetComponent<MapComponent>();
    initMapUtility(&m_maps, mapComp.map, m_pather);
}


void Game::textureLoadFailed(const Nz::String& file)
{
    Nz::StringStream errLog;
    errLog << __FILE__ << ' ' << __FUNCTION__ << " l" << __LINE__ << ": \n";
    errLog << "PANIC ! Reason: Texture loading failed - File: " << file;

    NazaraError(errLog);
}

void Game::addTextures()
{
    m_textures.setPrefix("./../../data/img/");

    std::vector<std::pair<Nz::String, Nz::String>> filepaths // Todo: Make an additional textures file
    {
        { ":/game/money", "game/main/money.png" },
        { ":/game/scheme", "game/main/scheme.png" },
        { ":/game/teal", "game/main/teal.png" },
        { Def::DEFAULTMAPTILESET, "game/main/tileset.png" },
        { ":/game/char/villager", "game/char/villager.png" }

        //...
    };

    for (auto& pair : filepaths)
        if (!m_textures.addByLoad(pair.first, pair.second))
            textureLoadFailed(pair.second);

    // Load custom textures
    // Custom textures can be used in custom mods
    // TODO: Custom mods

    // First, checks if it exists
    if (!Nz::File::Exists("./../../data/addons/additional_textures"))
        return;

    // Now, open it
    Nz::File customTextures;

    if (!customTextures.Open("./../../data/addons/additional_textures", 
                             Nz::OpenMode_ReadOnly | Nz::OpenMode_Text))
    {
        NazaraError("Cannot open custom textures file");
        return;
    }

    std::vector<Nz::String> customPair;

    while (!customTextures.EndOfFile())
    {
        auto line = customTextures.ReadLine();

        if (line.StartsWith("#") || // It's a comment
            line.IsEmpty() || line == ' ') // Empty line
            continue;

        customPair.clear();

        if(line.Split(customPair, " ; ") != 2u)
            continue; // Need 2 values

        if (!m_textures.addByLoad(customPair[0], customPair[1]))
            textureLoadFailed(customPair[1]);
    }
}

void Game::initTilesetCore()
{
    m_tilesetCore.add(0u, "concrete");
    m_tilesetCore.add(1u, "grass");
    m_tilesetCore.add(2u, "sand");
    m_tilesetCore.add(3u, "water");
}

void Game::addMaps() /// TODO: Load from JSON
{
    Nz::String tilesTexture = m_textures.get(Def::DEFAULTMAPTILESET)->GetFilePath();

    MapData map0_0
    {
        /*{
            2, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3,
            1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
            1, 1, 1, 2, 1, 2, 2, 2, 3, 2, 3, 3, 3, 3, 3,
            1, 1, 1, 1, 2, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
            1, 1, 1, 2, 1, 1, 2, 2, 1, 2, 2, 2, 2, 2, 3,
            0, 0, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2,
            0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 1, 1, 2, 2, 2,
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2
        },*/

        {
            "sand", "grass", "sand", "sand", "sand", "sand", "water", "water", "water", "water", "water", "water", "water", "water", "water",
            "grass", "sand", "sand", "sand", "sand", "sand", "sand", "water", "water", "water", "water", "water", "water", "water", "water",
            "grass", "grass", "grass", "sand", "grass", "sand", "sand", "sand", "water", "sand", "water", "water", "water", "water", "water",
            "grass", "grass", "grass", "grass", "sand", "grass", "grass", "sand", "sand", "sand", "sand", "water", "water", "water", "water",
            "grass", "grass", "grass", "sand", "grass", "grass", "sand", "sand", "grass", "sand", "sand", "sand", "sand", "sand", "water",
            "concrete", "concrete", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "grass", "sand", "sand", "sand", "sand", "sand",
            "concrete", "concrete", "concrete", "concrete", "grass", "grass", "grass", "grass", "grass", "sand", "grass", "grass", "sand", "sand", "sand",
            "concrete", "concrete", "concrete", "concrete", "concrete", "concrete", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand"
        },

        {
            0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        }
    };

    m_maps.add({ 0, 0 }, map0_0);

    MapData map1_0
    {
        /*{
            0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2
        },*/

        {
            "concrete", "concrete", "concrete", "grass", "sand", "sand", "sand", "sand", "sand", "sand", "sand", "sand", "sand", "sand", "sand",
            "concrete", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand", "sand", "sand", "sand", "sand", "sand",
            "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand", "sand", "sand", "sand", "sand",
            "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand", "sand", "sand", "sand",
            "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand", "sand",
            "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand", "sand",
            "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand",
            "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand"
        },

        {}
    };

    m_maps.add({ 1, 0 }, map1_0);
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
    Ndk::InitializeComponent<MapPositionComponent>("mappos");

    Ndk::InitializeComponent<Items::HPGainComponent>("hpgain");
    Ndk::InitializeComponent<Items::AttackBonusComponent>("atkbonus");
    Ndk::InitializeComponent<Items::AttackResistanceComponent>("atkres");
    Ndk::InitializeComponent<Items::ItemComponent>("item");
    Ndk::InitializeComponent<Items::EquippableComponent>("equip");
    Ndk::InitializeComponent<Items::EdibleComponent>("edible");
    Ndk::InitializeComponent<Items::ResourceComponent>("res");

    //Systems
    Ndk::InitializeSystem<AISystem>();
    Ndk::InitializeSystem<MovementSystem>();
    Ndk::InitializeSystem<AnimationSystem>();
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
    m_map = m_world->CreateEntity();
    
    auto& mapComp = m_map->AddComponent<MapComponent>();
    mapComp.init(*m_maps.get({ 0, 0 }), m_textures.get(Def::DEFAULTMAPTILESET)->GetFilePath(),
                 &m_tilesetCore);

    m_pather = std::make_shared<micropather::MicroPather>(mapComp.map.get(), Def::MAPX * Def::MAPY, 8);


    Nz::MaterialRef charMat = Nz::Material::New();
    charMat->LoadFromFile( m_textures.get(":/game/char/villager")->GetFilePath() );

    charMat->EnableBlending(true);
    charMat->SetDstBlend(Nz::BlendFunc_InvSrcAlpha);
    charMat->SetSrcBlend(Nz::BlendFunc_SrcAlpha);
    charMat->EnableDepthWrite(false);

    Nz::SpriteRef charSprite = Nz::Sprite::New(charMat);
    charSprite->SetTextureRect({ 0u, 0u, 113u, 99u });


    CharacterData mainCharacData { { 113u, 99u }, charSprite,
                                     15, { -25.f, -66.f }, { 1, 1 }, 100u };

    m_charac = make_character(m_world, mainCharacData);
}

void Game::addSystems()
{
    m_world->AddSystem<AISystem>(m_pather);
    m_world->AddSystem<MovementSystem>();
    m_world->AddSystem<AnimationSystem>();
}

void Game::initEventHandler()
{
    auto& eventHandler = m_window.GetEventHandler();

    m_mouseButtonEvent.Connect(eventHandler.OnMouseButtonPressed,
    [this] (const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent& event)
    { // Lambda to move the player if the user clicked in the map
        if (m_mapViewport.Contains(event.x, event.y))
        {
            auto& posComp = m_charac->GetComponent<PositionComponent>();
            auto& moveComp = m_charac->GetComponent<MoveToComponent>();

            auto lpos = getTileFromGlobalCoords({ event.x, event.y });
            auto diff = AbsPosToDiff({ posComp.x, posComp.y }, lpos);

            moveComp.diffX = diff.x;
            moveComp.diffY = diff.y;
        }
    });
}
