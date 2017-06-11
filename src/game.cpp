// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "game.hpp"

Game::Game(Ndk::Application& app, const Nz::Vector2ui& winSize,
           const Nz::Vector2ui& viewport, const Nz::String& winName)
    : m_app(app), m_window(app.AddWindow<Nz::RenderWindow>()), m_mapViewport(viewport)
{
    addTextures();

    Nz::ImageRef scheme = Nz::Image::New();
    scheme->LoadFromFile(Nz::TextureLibrary::Get(":/game/scheme")->GetFilePath());

    m_window.Create(Nz::VideoMode(winSize.x, winSize.y, 32), winName);
    m_window.EnableVerticalSync(true);

    m_world = app.AddWorld().CreateHandle();
    m_canvas = std::make_unique<Ndk::Canvas>(m_world->CreateHandle(), m_window.GetEventHandler(), m_window.GetCursorController().CreateHandle());

    initSchemeUtility(scheme);
    initNazara();

    initTilesetCore();
    addMaps();

    initIcon();
    initCam();

    addEntities();
    addSystems();
    initEventHandler();
    addWidgets();
    addPauseMenu();

    auto& mapComp = m_map->GetComponent<MapComponent>();
    initMapUtility(mapComp.map, m_pather, m_charac);

    auto testItem = make_item(m_world, "Legendary sword 1234"); // [TEST]
    testItem->AddComponent<AttackModifierComponent>(10);

    m_charac->GetComponent<InventoryComponent>().add(testItem); // [TEST]
}

#include <iostream> // [TEST]

void Game::showInventory(bool detail) // [TEST]
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

        if (detail)
        {
            if (item->HasComponent<AttackModifierComponent>())
            {
                auto& atk = item->GetComponent<AttackModifierComponent>();
                std::cout << "\n  Attack Modifier:\n";
                std::cout << "    Neutral: " << atk.neutral << '\n';
                std::cout << "    Air: " << atk.air << '\n';
                std::cout << "    Fire: " << atk.fire << '\n';
                std::cout << "    Water: " << atk.water << '\n';
                std::cout << "    Earth: " << atk.earth;
            }

            if (item->HasComponent<ResistanceModifierComponent>())
            {
                auto& res = item->GetComponent<ResistanceModifierComponent>();
                std::cout << "\n  Resistance Modifier:\n";
                std::cout << "    Neutral: " << res.neutral << '\n';
                std::cout << "    Air: " << res.air << '\n';
                std::cout << "    Fire: " << res.fire << '\n';
                std::cout << "    Water: " << res.water << '\n';
                std::cout << "    Earth: " << res.earth;
            }
        }

        std::cout << '\n';
        ++counter;
    }

    std::cout << std::flush;
}

void Game::showCaracteristics()
{
    std::cout << "Main character caracteristics";
    if (m_charac->HasComponent<AttackModifierComponent>())
    {
        auto& atk = m_charac->GetComponent<AttackModifierComponent>();
        std::cout << "\n  Attack Modifier:\n";
        std::cout << "    Neutral: " << atk.neutral << '\n';
        std::cout << "    Air: " << atk.air << '\n';
        std::cout << "    Fire: " << atk.fire << '\n';
        std::cout << "    Water: " << atk.water << '\n';
        std::cout << "    Earth: " << atk.earth;
    }

    if (m_charac->HasComponent<ResistanceModifierComponent>())
    {
        auto& res = m_charac->GetComponent<ResistanceModifierComponent>();
        std::cout << "\n  Resistance Modifier:\n";
        std::cout << "    Neutral: " << res.neutral << '\n';
        std::cout << "    Air: " << res.air << '\n';
        std::cout << "    Fire: " << res.fire << '\n';
        std::cout << "    Water: " << res.water << '\n';
        std::cout << "    Earth: " << res.earth;
    }
    std::cout << std::endl;
}

void Game::addTextures()
{
    static Nz::String rootPrefix =        "../data/";
    static Nz::String imgPrefix = rootPrefix +     "img/";
    static Nz::String addonsPrefix = rootPrefix +  "addons/";
        static Nz::String addonsImgPrefix = addonsPrefix + "imgs/";

    std::vector<std::pair<Nz::String, Nz::String>> filepaths
    {
        { ":/game/money", "game/main/money.png" },
        { ":/game/scheme", "game/main/scheme.png" },
        { ":/game/teal", "game/main/teal.png" },
        { Def::DEFAULTMAPTILESET, "game/main/tileset.png" },
        { ":/game/char/villager", "game/char/villager.png" }

        //...
    };

    for (auto& pair : filepaths)
        Nz::TextureLibrary::Register(pair.first, Nz::TextureManager::Get(imgPrefix + pair.second));

    // Load custom textures
    // Custom textures can be used in custom mods

    /// \todo Custom mods

    // First, checks if it exists
    if (!Nz::File::Exists(addonsPrefix + "additional_textures"))
        return;

    // Now, open it
    Nz::File customTextures;

    if (!customTextures.Open(addonsPrefix + "additional_textures",
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

        auto texture = Nz::TextureManager::Get(addonsImgPrefix + customPair[1]);

        if (!texture.IsValid())
        {
            NazaraDebug("Texture " + customPair[0] + " can't be loaded - wrong filepath.\n"
                        "Provided filepath = " + customPair[1]);
            continue;
        }

        Nz::TextureLibrary::Register(customPair[0], texture);
        NazaraDebug("Texture " + customPair[0] + " loaded !");
    }
}

void Game::initTilesetCore()
{
    unsigned concrete = 0;
    unsigned grass = concrete + 1;
    unsigned sand = grass + 1;
    unsigned water = sand + 1;

    m_tilesetCore.add(concrete, "cncrt");
    m_tilesetCore.add(grass, "grass");
    m_tilesetCore.add(sand, "sandy");
    m_tilesetCore.add(water, "water");
}

void Game::addMaps() /// \todo Load from file (lua)
{
    Nz::String tilesTexture = Nz::TextureLibrary::Get(Def::DEFAULTMAPTILESET)->GetFilePath();

    MapDataRef map0_0 = MapData::New();

    map0_0->setMap
    ({
        "sandy",  "sandy", "sandy", "water", "water", "water", "water", "water",
            "grass", "sandy", "sandy", "water", "water", "water", "water",
        "grass", "sandy", "sandy", "sandy", "water", "water", "water", "water",
            "sandy", "sandy", "sandy", "water", "water", "water", "water",
        "grass", "grass", "grass", "sandy", "water", "water", "water", "water",
            "grass", "sandy", "sandy", "sandy", "sandy", "water", "water",
        "grass", "grass", "sandy", "grass", "sandy", "sandy", "water", "water",
            "grass", "grass", "grass", "sandy", "sandy", "water", "water",
        "grass", "grass", "grass", "sandy", "grass", "sandy", "sandy", "water",
            "grass", "sandy", "grass", "sandy", "sandy", "sandy", "sandy",
        "cncrt", "grass", "grass", "grass", "sandy", "sandy", "sandy", "sandy",
            "cncrt", "grass", "grass", "grass", "grass", "sandy", "sandy",
        "cncrt", "cncrt", "grass", "grass", "grass", "grass", "sandy", "sandy",
            "cncrt", "cncrt", "grass", "grass", "sandy", "grass", "sandy",
        "cncrt", "cncrt", "cncrt", "grass", "grass", "grass", "grass", "sandy",
            "cncrt", "cncrt", "cncrt", "grass", "grass", "grass", "grass"
    });

    map0_0->setObs
    ({
        0, 0, 0, 1, 1, 1, 1, 1,
          0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 1, 1, 1, 1,
          0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 1, 1, 1, 1,
          0, 0, 0, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1,
          0, 0, 0, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 1,
          0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0
    });

    MapDataLibrary::Register("0;0", deactivateMapEntities(map0_0));


    MapDataRef map1_0 = MapData::New();

    map1_0->setMap
    ({
        "cncrt", "cncrt", "sandy", "sandy", "sandy", "sandy", "sandy", "sandy",
            "cncrt", "grass", "sandy", "sandy", "sandy", "sandy", "sandy",
        "cncrt", "grass", "grass", "grass", "sandy", "sandy", "sandy", "sandy",
            "grass", "grass", "grass", "grass", "sandy", "sandy", "sandy",
        "grass", "grass", "grass", "grass", "grass", "sandy", "sandy", "sandy",
            "grass", "grass", "grass", "grass", "sandy", "sandy", "sandy",
        "grass", "grass", "grass", "grass", "grass", "sandy", "sandy", "sandy",
            "grass", "grass", "grass", "grass", "grass", "sandy", "sandy",
        "grass", "grass", "grass", "grass", "grass", "grass", "sandy", "sandy",
            "grass", "grass", "grass", "grass", "grass", "grass", "sandy",
        "grass", "grass", "grass", "grass", "grass", "grass", "sandy", "sandy",
            "grass", "grass", "grass", "grass", "grass", "grass", "sandy",
        "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sandy",
            "grass", "grass", "grass", "grass", "grass", "grass", "sandy",
        "grass", "grass", "grass", "grass", "grass", "grass", "grass", "sandy",
            "grass", "grass", "grass", "grass", "grass", "grass", "grass"
    });


    Nz::MaterialRef npcMat = Nz::Material::New();
    npcMat->Configure("Translucent2D");
    npcMat->SetDiffuseMap(Nz::TextureLibrary::Get(":/game/char/villager"));

    Nz::SpriteRef npcSprite = Nz::Sprite::New(npcMat);
    npcSprite->SetTextureRect({ 0u, 0u, 113u, 99u });

    CharacterData npcData
    {
        { 113u, 99u }, npcSprite, 15, { -25.f, -66.f }, { 5, 5 }, { 1, 0 }, 100u,
        AnimationComponent::OnMove, Orientation::DownLeft, { true }, true, "The Wandering NPC"
    };

    auto npc = make_character(m_world, npcData);
    map1_0->addEntity(npc);

    MapDataLibrary::Register("1;0", deactivateMapEntities(map1_0));


    MapDataRef map0_1 = MapData::New();

    map0_1->setMap
    ({
        "water", "water", "water", "water", "water", "water", "water", "water",
            "water", "water", "water", "water", "water", "water", "water",
        "water", "water", "water", "water", "water", "sandy", "water", "water",
            "water", "water", "water", "water", "sandy", "sandy", "water",
        "water", "water", "water", "water", "sandy", "sandy", "sandy", "water",
            "water", "water", "water", "water", "sandy", "sandy", "water",
        "water", "water", "water", "water", "water", "sandy", "water", "water",
            "water", "water", "water", "water", "water", "water", "water",
        "sandy", "sandy", "water", "water", "water", "water", "water", "water",
            "sandy", "water", "water", "water", "water", "water", "water",
        "sandy", "sandy", "water", "water", "water", "water", "water", "water",
            "sandy", "sandy", "water", "water", "water", "water", "water",
        "sandy", "sandy", "sandy", "water", "water", "water", "water", "water",
            "sandy", "sandy", "water", "water", "water", "water", "water",
        "grass", "sandy", "sandy", "water", "water", "water", "water", "water",
            "sandy", "sandy", "sandy", "water", "water", "water", "water"
    });

    map0_1->setObs
    ({
        1, 1, 1, 1, 1, 1, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 1, 1,
          1, 1, 1, 1, 0, 0, 1,
        1, 1, 1, 1, 0, 0, 0, 1,
          1, 1, 1, 1, 0, 0, 1,
        1, 1, 1, 1, 1, 0, 1, 1,
          1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1, 1,
          0, 1, 1, 1, 1, 1, 1,
        0, 0, 1, 1, 1, 1, 1, 1,
          0, 0, 1, 1, 1, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 1,
          0, 0, 1, 1, 1, 1, 1,
        0, 0, 0, 1, 1, 1, 1, 1,
          0, 0, 0, 1, 1, 1, 1
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
    Ndk::InitializeComponent<OrientationComponent>("orient");
    Ndk::InitializeComponent<AnimationComponent>("anim");
    Ndk::InitializeComponent<PositionComponent>("pos");
    Ndk::InitializeComponent<MoveComponent>("move");
    Ndk::InitializeComponent<PathComponent>("path");
    Ndk::InitializeComponent<FightComponent>("fight");
    Ndk::InitializeComponent<LifeComponent>("life");
    Ndk::InitializeComponent<MapComponent>("map");
    Ndk::InitializeComponent<MapPositionComponent>("mappos");
    Ndk::InitializeComponent<AttackModifierComponent>("atkbonus");
    Ndk::InitializeComponent<ResistanceModifierComponent>("atkres");
    Ndk::InitializeComponent<DescriptionComponent>("desc");
    Ndk::InitializeComponent<CombatBehaviorComponent>("cbtbhv");
    Ndk::InitializeComponent<BlockTileComponent>("blcktile");
    Ndk::InitializeComponent<MonsterComponent>("monster");
    Ndk::InitializeComponent<EquipmentComponent>("equip");

    Ndk::InitializeComponent<Items::HPGainComponent>("hpgain");
    Ndk::InitializeComponent<Items::ItemComponent>("item");
    Ndk::InitializeComponent<Items::EquippableComponent>("iequip");
    Ndk::InitializeComponent<Items::EdibleComponent>("edible");
    Ndk::InitializeComponent<Items::ResourceComponent>("res");

    // Systems
    Ndk::InitializeSystem<AISystem>();
    Ndk::InitializeSystem<MovementSystem>();
    Ndk::InitializeSystem<RandomMovementSystem>();
    Ndk::InitializeSystem<AnimationSystem>();
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
    charMat->Configure("Translucent2D");
    charMat->SetDiffuseMap(Nz::TextureLibrary::Get(":/game/char/villager"));

    Nz::SpriteRef charSprite = Nz::Sprite::New(charMat);
    charSprite->SetTextureRect({ 0u, 0u, 113u, 99u });


    CharacterData mainCharacData
    {
        { 113u, 99u }, charSprite, 15, { -25.f, -66.f }, { 1, 1 }, { 0, 0 }, 100u, AnimationComponent::OnMove, Orientation::Down, {}, false, "me", "", { 0, 0, 100, 0, 0 }
    };

    m_charac = make_character(m_world, mainCharacData);
    m_charac->GetComponent<Ndk::NodeComponent>().Move(0, 0, -1);
}

void Game::addSystems()
{
    m_world->AddSystem<AISystem>(m_pather);
    m_world->AddSystem<MovementSystem>();
    m_world->AddSystem<RandomMovementSystem>();
    m_world->AddSystem<AnimationSystem>();
}

void Game::initEventHandler()
{
    auto& eventHandler = m_window.GetEventHandler();

    m_mouseButtonEvent.Connect(eventHandler.OnMouseButtonPressed,
    [this] (const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent& event)
    { // Lambda to move the player if the user clicked in the map
        if (m_mapViewport.Contains(event.x, event.y) && !m_paused)
        {
            auto& move = m_charac->GetComponent<MoveComponent>();

            auto tile = getTileFromGlobalCoords({ event.x, event.y });

            move.tile = tile;
            move.playerInitiated = true;
        }
    });

    m_keyPressEvent.Connect(eventHandler.OnKeyPressed,
    [this] (const Nz::EventHandler*, const Nz::WindowEvent::KeyEvent& event)
    {
        if (event.code == Nz::Keyboard::Escape) // Pause menu
            enablePauseMenu(!m_paused);

        if (m_paused)
            return;

        switch (event.code)
        {
        case Nz::Keyboard::I: // Inventory
            showInventory(event.shift);
            break;

        case Nz::Keyboard::C: // Caracteristics
            showCaracteristics();
            break;
        }
    });
}

void Game::addWidgets()
{
    NazaraAssert(m_canvas, "Canvas null");
    m_canvas->SetPosition(10.f, static_cast<float>(Def::MAPYVIEWPORT) + 5);

    auto& eventHandler = m_window.GetEventHandler();

    Ndk::ButtonWidget* invButton = m_canvas->Add<Ndk::ButtonWidget>();
    NazaraAssert(invButton, "Inventory button null");

    invButton->SetParent(*m_canvas);
    invButton->UpdateText(Nz::SimpleTextDrawer::Draw("Inventory", 15));
    invButton->SetPadding(0.f, 0.f, 0.f, 0.f);
    invButton->ResizeToContent();

    m_invButtonEvent.Connect(invButton->OnButtonTrigger,
    [this] (const Ndk::ButtonWidget*)
    {
        if (!m_paused)
            showInventory();
    });
}

void Game::addPauseMenu()
{
    // Background
    m_background = m_world->CreateEntity();
    m_background->AddComponent<Ndk::NodeComponent>();

    auto& gfxBG = m_background->AddComponent<Ndk::GraphicsComponent>();

    Nz::Color colorBG(0, 0, 0, 170);

    Nz::SpriteRef spriteBG = Nz::Sprite::New();

    spriteBG->GetMaterial()->Configure("Translucent2D");
    spriteBG->SetColor(colorBG);
    spriteBG->SetSize(static_cast<float>(Def::WINXSIZE), static_cast<float>(Def::WINYSIZE));

    gfxBG.Attach(spriteBG, Def::PAUSE_MENU_BACKGROUND_LAYER);

    // Pause Text
    m_pauseText = m_world->CreateEntity();
    m_pauseText->AddComponent<Ndk::NodeComponent>();

    auto& gfxPause = m_pauseText->AddComponent<Ndk::GraphicsComponent>();

    Nz::TextSpriteRef textPause = Nz::TextSprite::New();
    textPause->Update(Nz::SimpleTextDrawer::Draw("Pause menu\n"
                                                 "ESC = Toggle Pause Menu\n"
                                                 "(Shift) I = Inventory\n"
                                                 "C = Caracteristics", 20));

    gfxPause.Attach(textPause, Def::PAUSE_MENU_BUTTONS_LAYER);

    auto& nodePause = m_pauseText->GetComponent<Ndk::NodeComponent>();
    Nz::Boxf textBox = gfxPause.GetBoundingVolume().aabb;

    nodePause.SetPosition(Def::WINXSIZE / 2 - textBox.width / 2, 20);

    enablePauseMenu(false);
}

void Game::enablePauseMenu(bool enable)
{
    m_paused = enable;
    m_background->Enable(enable);
    m_pauseText->Enable(enable);

    m_world->GetSystem<AISystem>().Enable(!enable);
    m_world->GetSystem<MovementSystem>().Enable(!enable);
    m_world->GetSystem<AnimationSystem>().Enable(!enable);
    m_world->GetSystem<RandomMovementSystem>().Enable(!enable);
}
