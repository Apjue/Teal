// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "game.hpp"

Game::Game(Ndk::Application& app, const Nz::Vector2ui& winSize,
           const Nz::Vector2ui& viewport, const Nz::String& winName) :
    m_app(app), m_world(), m_window(app.AddWindow<Nz::RenderWindow>()), m_map(), m_charac(),
    m_mapViewport(viewport)
{
    addTextures();

    Nz::ImageRef scheme = Nz::Image::New();
    scheme->LoadFromFile(Nz::TextureLibrary::Get(":/game/scheme")->GetFilePath());

    initSchemeUtility(scheme);

    m_window.Create(Nz::VideoMode(winSize.x, winSize.y, 32), winName);
    m_world = app.AddWorld().CreateHandle();

    initNazara();

    initTilesetCore();
    addMaps();

    initIcon();
    initCam();

    addEntities();
    addSystems();
    initEventHandler();

    auto& mapComp = m_map->GetComponent<MapComponent>();
    initMapUtility(mapComp.map, m_pather, m_charac);

    auto testItem = make_item(m_world, "Legendary sword 1234"); // [TEST]

    m_charac->GetComponent<InventoryComponent>().add(testItem); // [TEST]
}

#include <iostream> // [TEST]

void Game::showInventory() // [TEST]
{
    auto& inv = m_charac->GetComponent<InventoryComponent>();

    unsigned counter {};
    for (auto& item : inv.getAll())
    {
        std::cout << "Item #" << counter << ". ";

        if (item->HasComponent<NameComponent>())
        {
            auto& name = item->GetComponent<NameComponent>();
            std::cout << "Name: " << name.name;
        }

        std::cout << '\n';
        ++counter;
    }

    std::cout << std::flush;
}

void Game::addTextures()
{
    Nz::String prefix = "../data/img/";

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
        Nz::TextureLibrary::Register(pair.first, Nz::TextureManager::Get(prefix + pair.second));

    // Load custom textures
    // Custom textures can be used in custom mods

    /// \todo Custom mods

    // First, checks if it exists
    if (!Nz::File::Exists(prefix + "../addons/additional_textures"))
        return;

    // Now, open it
    Nz::File customTextures;

    if (!customTextures.Open(prefix + "../addons/additional_textures",
                             Nz::OpenMode_ReadOnly | Nz::OpenMode_Text))
    {
        NazaraError("Cannot open custom textures file");
        return;
    }

    if (customTextures.GetSize() == 0)
        return; // Empty file

    std::vector<Nz::String> customPair;

    while (!customTextures.EndOfFile())
    {
        auto line = customTextures.ReadLine();

        if (line.StartsWith("#") || // Comment
            line.IsEmpty() || line == ' ') // Empty line
            continue;

        customPair.clear();

        if (line.Split(customPair, " ; ") != 2u)
            continue; // Need 2 values

        Nz::TextureLibrary::Register(customPair[0], Nz::TextureManager::Get(prefix + customPair[1]));
    }
}

void Game::initTilesetCore()
{
    unsigned concrete = 0;
    unsigned grass = concrete + 1;
    unsigned sand = grass + 1;
    unsigned water = sand + 1;

    m_tilesetCore.add(concrete, "concrete");
    m_tilesetCore.add(grass, "grass");
    m_tilesetCore.add(sand, "sand");
    m_tilesetCore.add(water, "water");
}

void Game::addMaps() /// \todo Load from file (lua?)
{
    Nz::String tilesTexture = Nz::TextureLibrary::Get(Def::DEFAULTMAPTILESET)->GetFilePath();

    MapDataRef map0_0 = MapData::New();

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

    map0_0->setMap({
        "sand", "grass", "sand", "sand", "sand", "sand", "water", "water", "water", "water", "water", "water", "water", "water", "water",
        "grass", "sand", "sand", "sand", "sand", "sand", "sand", "water", "water", "water", "water", "water", "water", "water", "water",
        "grass", "grass", "grass", "sand", "grass", "sand", "sand", "sand", "water", "sand", "water", "water", "water", "water", "water",
        "grass", "grass", "grass", "grass", "sand", "grass", "grass", "sand", "sand", "sand", "sand", "water", "water", "water", "water",
        "grass", "grass", "grass", "sand", "grass", "grass", "sand", "sand", "grass", "sand", "sand", "sand", "sand", "sand", "water",
        "concrete", "concrete", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "grass", "sand", "sand", "sand", "sand", "sand",
        "concrete", "concrete", "concrete", "concrete", "grass", "grass", "grass", "grass", "grass", "sand", "grass", "grass", "sand", "sand", "sand",
        "concrete", "concrete", "concrete", "concrete", "concrete", "concrete", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand"
    });

    map0_0->setObs({
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    });

    MapDataLibrary::Register("0;0", deactivateMapEntities(map0_0));


    MapDataRef map1_0 = MapData::New();

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

    map1_0->setMap({
        "concrete", "concrete", "concrete", "grass", "sand", "sand", "sand", "sand", "sand", "sand", "sand", "sand", "sand", "sand", "sand",
        "concrete", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand", "sand", "sand", "sand", "sand", "sand",
        "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand", "sand", "sand", "sand", "sand",
        "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand", "sand", "sand", "sand",
        "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand", "sand",
        "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand", "sand",
        "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand", "sand",
        "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sand"
    });


    Nz::MaterialRef npcMat = Nz::Material::New(); // [TEST]
    npcMat->LoadFromFile(Nz::TextureLibrary::Get(":/game/char/villager")->GetFilePath());

    npcMat->EnableBlending(true);
    npcMat->SetDstBlend(Nz::BlendFunc_InvSrcAlpha);
    npcMat->SetSrcBlend(Nz::BlendFunc_SrcAlpha);
    npcMat->EnableDepthWrite(false);

    Nz::SpriteRef npcSprite = Nz::Sprite::New(npcMat);
    npcSprite->SetTextureRect({ 0u, 0u, 113u, 99u });

    CharacterData npcData // [TEST]
    {
        { 113u, 99u }, npcSprite, 15, { -25.f, -66.f }, { 5, 5 }, { 1, 0 }, 100u,
        AnimationComponent::OnMove, Orientation::DownLeft, { true }, "The Wandering NPC"
    };

    auto npc = make_character(m_world, npcData);
    map1_0->addEntity(npc);

    MapDataLibrary::Register("1;0", deactivateMapEntities(map1_0));


    MapDataRef map0_1 = MapData::New();

    map0_1->setMap({
        "water", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water",
        "water", "water", "water", "water", "water", "water", "water", "water", "water", "sand", "sand", "sand", "water", "water", "water",
        "water", "water", "water", "water", "water", "water", "water", "water", "sand", "sand", "sand", "sand", "sand", "water", "water",
        "water", "water", "water", "water", "water", "water", "water", "water", "water", "water", "sand", "water", "water", "water", "water",
        "sand", "sand", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water",
        "sand", "sand", "sand", "sand", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water",
        "sand", "sand", "sand", "sand", "sand", "water", "water", "water", "water", "water", "water", "water", "water", "water", "water",
        "grass", "sand", "sand", "sand", "sand", "sand", "water", "water", "water", "water", "water", "water", "water", "water", "water"
    });

    map0_1->setObs({
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
        0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1
    });

    MapDataLibrary::Register("0;1", deactivateMapEntities(map0_1));
}

void Game::initNazara()
{
    // Components
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
    Ndk::InitializeComponent<AttackModifierComponent>("atkbonus");
    Ndk::InitializeComponent<ResistanceModifierComponent>("atkres");
    Ndk::InitializeComponent<DescriptionComponent>("desc");

    Ndk::InitializeComponent<Items::HPGainComponent>("hpgain");
    Ndk::InitializeComponent<Items::ItemComponent>("item");
    Ndk::InitializeComponent<Items::EquippableComponent>("equip");
    Ndk::InitializeComponent<Items::EdibleComponent>("edible");
    Ndk::InitializeComponent<Items::ResourceComponent>("res");

    // Systems
    Ndk::InitializeSystem<AISystem>();
    Ndk::InitializeSystem<MovementSystem>();
    Ndk::InitializeSystem<AnimationSystem>();
    Ndk::InitializeSystem<RandomMovementSystem>();
}

void Game::initIcon()
{
    Nz::Image iconImage;
    iconImage.LoadFromFile(Nz::TextureLibrary::Get(":/game/money")->GetFilePath());

    m_winIcon = Nz::Icon::New();
    m_winIcon->Create(iconImage);

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
    mapComp.init(MapDataLibrary::Get("0;0"), Nz::TextureLibrary::Get(Def::DEFAULTMAPTILESET)->GetFilePath(),
                 &m_tilesetCore);

    activateMapEntities(MapDataLibrary::Get("0;0"));

    m_pather = std::make_shared<micropather::MicroPather>(mapComp.map.get(), Def::MAPX * Def::MAPY, 8);


    Nz::MaterialRef charMat = Nz::Material::New();
    charMat->LoadFromFile(Nz::TextureLibrary::Get(":/game/char/villager")->GetFilePath() );

    charMat->EnableBlending(true);
    charMat->SetDstBlend(Nz::BlendFunc_InvSrcAlpha);
    charMat->SetSrcBlend(Nz::BlendFunc_SrcAlpha);
    charMat->EnableDepthWrite(false);

    Nz::SpriteRef charSprite = Nz::Sprite::New(charMat);
    charSprite->SetTextureRect({ 0u, 0u, 113u, 99u });


    CharacterData mainCharacData
    {
        { 113u, 99u }, charSprite, 15, { -25.f, -66.f }, { 1, 1 }, { 0, 0 }, 100u
    };

    m_charac = make_character(m_world, mainCharacData);
}

void Game::addSystems()
{
    m_world->AddSystem<AISystem>(m_pather);
    m_world->AddSystem<MovementSystem>();
    m_world->AddSystem<AnimationSystem>();
    m_world->AddSystem<RandomMovementSystem>();
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

    m_keyPressEvent.Connect(eventHandler.OnKeyPressed,
    [this] (const Nz::EventHandler*, const Nz::WindowEvent::KeyEvent& event)
    {
        switch (event.code)
        {
        case Nz::Keyboard::I: // Inventory
            showInventory();
        }
    });
}
