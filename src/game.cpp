// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <iostream> // [TEST]
#include "game.hpp"

Game::Game(Ndk::Application& app, const Nz::Vector2ui& winSize,
           const Nz::Vector2ui& viewport, const Nz::String& winName)
    : m_app(app), m_window(app.AddWindow<Nz::RenderWindow>()), m_mapViewport(viewport)
{
    loadTextures();

    Nz::ImageRef scheme = Nz::Image::New();
    scheme->LoadFromFile(Nz::TextureLibrary::Get(":/game/scheme")->GetFilePath());

    m_window.Create(Nz::VideoMode(winSize.x, winSize.y, 32), winName);
    m_window.EnableVerticalSync(true);

    m_world = app.AddWorld().CreateHandle();
    m_canvas = std::make_unique<Ndk::Canvas>(m_world->CreateHandle(), m_window.GetEventHandler(), m_window.GetCursorController().CreateHandle());

    initSchemeUtility(scheme);
    initNazara();

    initTilesetCore();
    loadMaps();

    initIcon();
    initCam();

    addEntities();
    addSystems();

    loadStates();
    loadSkills();
    loadItems();

    initEventHandler();
    addWidgets();
    addPauseMenu();

    auto& mapComp = m_map->GetComponent<MapComponent>();
    initMapUtility(mapComp.map, m_pather, m_charac);
}

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
                std::cout << "    Neutral: " << atk.data[Element::Neutral] << '\n';
                std::cout << "    Air: " << atk.data[Element::Air] << '\n';
                std::cout << "    Fire: " << atk.data[Element::Fire] << '\n';
                std::cout << "    Water: " << atk.data[Element::Water] << '\n';
                std::cout << "    Earth: " << atk.data[Element::Earth];
            }

            if (item->HasComponent<ResistanceModifierComponent>())
            {
                auto& res = item->GetComponent<ResistanceModifierComponent>();
                std::cout << "\n  Resistance Modifier:\n";
                std::cout << "    Neutral: " << res.data[Element::Neutral] << '\n';
                std::cout << "    Air: " << res.data[Element::Air] << '\n';
                std::cout << "    Fire: " << res.data[Element::Fire] << '\n';
                std::cout << "    Water: " << res.data[Element::Water] << '\n';
                std::cout << "    Earth: " << res.data[Element::Earth];
            }
        }

        std::cout << '\n';
        ++counter;
    }

    std::cout << std::flush;
}

void Game::showCharacteristics() // [TEST]
{
    std::cout << "Main character caracteristics";
    if (m_charac->HasComponent<AttackModifierComponent>())
    {
        auto& atk = m_charac->GetComponent<AttackModifierComponent>();
        std::cout << "\n  Attack Modifier:\n";
        std::cout << "    Neutral: " << atk.data[Element::Neutral] << '\n';
        std::cout << "    Air: " << atk.data[Element::Air] << '\n';
        std::cout << "    Fire: " << atk.data[Element::Fire] << '\n';
        std::cout << "    Water: " << atk.data[Element::Water] << '\n';
        std::cout << "    Earth: " << atk.data[Element::Earth];
    }

    if (m_charac->HasComponent<ResistanceModifierComponent>())
    {
        auto& res = m_charac->GetComponent<ResistanceModifierComponent>();
        std::cout << "\n  Resistance Modifier:\n";
        std::cout << "    Neutral: " << res.data[Element::Neutral] << '\n';
        std::cout << "    Air: " << res.data[Element::Air] << '\n';
        std::cout << "    Fire: " << res.data[Element::Fire] << '\n';
        std::cout << "    Water: " << res.data[Element::Water] << '\n';
        std::cout << "    Earth: " << res.data[Element::Earth];
    }
    std::cout << std::endl;
}

void Game::loadTextures()
{
    std::vector<std::pair<Nz::String, Nz::String>> filepaths
    {
        { ":/game/money", "game/main/money.png" },
        { ":/game/scheme", "game/main/scheme.png" },
        { ":/game/teal", "game/main/teal.png" },
        { Def::DEFAULTMAPTILESET, "game/main/tileset.png" },
        { ":/game/fight_tileset", "game/main/tileset_fight.png" },
        { ":/game/char/villager", "game/char/villager.png" }

        //...
    };

    for (auto& pair : filepaths)
        Nz::TextureLibrary::Register(pair.first, Nz::TextureManager::Get(m_imgPrefix + pair.second));

    // Load custom textures
    // Custom textures can be used in custom mods

    /// \todo Custom mods

    // First, checks if it exists
    if (!Nz::File::Exists(m_addonsPrefix + "additional_textures"))
        return;

    // Now, open it
    Nz::File customTextures;

    if (!customTextures.Open(m_addonsPrefix + "additional_textures",
                             Nz::OpenMode_ReadOnly | Nz::OpenMode_Text))
    {
        NazaraNotice("Cannot open custom textures file");
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

        auto texture = Nz::TextureManager::Get(m_addonsImgPrefix + customPair[1]);

        if (!texture.IsValid())
        {
            NazaraError("Texture " + customPair[0] + " can't be loaded - wrong filepath.\n"
                        "Provided filepath = " + customPair[1]);
            continue;
        }

        Nz::TextureLibrary::Register(customPair[0], texture);
        NazaraDebug("Texture " + customPair[0] + " loaded !");
    }
}

void Game::initTilesetCore()
{
    TealException(Nz::File::Exists(m_scriptPrefix + "tilesetcore.lua"), "tilesetcore.lua not found !");

    {
        Nz::LuaInstance lua;
        TealException(lua.ExecuteFromFile(m_scriptPrefix + "tilesetcore.lua"), "Lua: tilesetcore.lua loading failed !");

        lua.GetGlobal("teal_tilesetcore");
        TealException(lua.GetType(-1) == Nz::LuaType_Table, "Lua: teal_tilesetcore isn't a table !");

        unsigned tileNumber {};

        for (int i { 1 };; ++i)
        {
            lua.PushInteger(i);

            if (lua.GetTable() != Nz::LuaType_Table)
            {
                lua.Pop();
                break;
            }

            m_tilesetCore.add(lua.CheckField<unsigned>("index"), lua.CheckField<Nz::String>("name"));
            ++tileNumber;

            lua.Pop();
        }

        Def::TILESETSIZE = static_cast<float>(tileNumber) * 64.f;
    }

    {
        Nz::LuaInstance lua;
        TealException(lua.ExecuteFromFile(m_scriptPrefix + "tilesetcore.lua"), "Lua: tilesetcore.lua loading failed !");

        lua.GetGlobal("teal_fighttilesetcore");
        TealException(lua.GetType(-1) == Nz::LuaType_Table, "Lua: teal_fighttilesetcore isn't a table !");

        unsigned tileNumber {};

        for (int i { 1 };; ++i)
        {
            lua.PushInteger(i);

            if (lua.GetTable() != Nz::LuaType_Table)
            {
                lua.Pop();
                break;
            }
            
            m_fightTilesetCore.add(lua.CheckField<unsigned>("index"), lua.CheckField<Nz::String>("name"));
            ++tileNumber;

            lua.Pop();
        }

        Def::FIGHTTILESETSIZE = static_cast<float>(tileNumber) * 64.f;
    }
}

void Game::loadMaps() /// \todo Load from file (lua)
{
    Nz::Directory maps { m_scriptPrefix + "maps/" };
    maps.SetPattern("*.lua");
    maps.Open();

    while (maps.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(maps.GetResultPath()))
        {
            NazaraError("Error loading map " + maps.GetResultName());
            TealException(maps.GetResultName() != "map0_0.lua", "Lua: starting map fails to load");
            continue;
        }

        // Fun starts
        TealException(lua.GetGlobal("teal_map") == Nz::LuaType_Table, "Lua: teal_map isn't a table !");

        MapDataRef map = MapData::New();
        TILEARRAY tiles;

        for (int i { 1 }; i <= Def::TILEARRAYSIZE; ++i)
        {
            lua.PushInteger(i);
            lua.GetTable();

            TealException(lua.GetType(-1) == Nz::LuaType_Table, Nz::String { "Lua: teal_map." } + i + " isn't a table !");

            tiles[i - 1].textureId = lua.CheckField<Nz::String>("textureId");
            tiles[i - 1].fightTextureId = lua.CheckField<Nz::String>("fightTextureId");

            tiles[i - 1].obstacle = lua.CheckField<unsigned>("obstacle");
            tiles[i - 1].visible = lua.CheckField<bool>("visible");

            lua.Pop();
        }

        map->setTiles(tiles);

        lua.PushString("entities");
        lua.GetTable();

        TealException(lua.GetType(-1) == Nz::LuaType_Table, "Lua: teal_map.entities isn't a table !");

        for (int i { 1 };; ++i)
        {
            lua.PushInteger(i);

            if (lua.GetTable() != Nz::LuaType_Table)
            {
                lua.Pop();
                break;
            }

            // get entities...
            // not just name of entities: pos in map too

            lua.Pop();
        }

        lua.Pop();

        MapDataLibrary::Register(lua.CheckField<Nz::String>("pos"), deactivateMapEntities(map)); // x;y
        NazaraDebug("Lua: loaded map " + maps.GetResultName() + " at pos " + lua.CheckField<Nz::String>("pos"));
    }
    
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
    npc->Enable(false);
    MapDataLibrary::Get("1;0")->getEntities().Insert(npc);
}

void Game::loadStates()
{
    StateMetaData poison { "Poisonned", "You are poisonned. Life sucks." };
    m_states.addSkill(PoisonnedState::getMetadataID(), poison);

    StateMetaData regen { "Regeneration", "You are healed. Life is cool." };
    m_states.addSkill(HealedState::getMetadataID(), regen);
}

void Game::loadSkills()
{
    /// \todo Load skills in lua

    // test
    SkillData s;
    s.attackList.push_back(std::make_shared<DamageData>(Element::Fire, 100));

    m_skills.addSkill("item_excalibur", s);
    
    Nz::Directory skills { m_scriptPrefix + "skills/" };
    skills.SetPattern("*.lua");
    skills.Open();

    while (skills.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(skills.GetResultPath()))
        {
            NazaraError("Error loading item " + skills.GetResultName());
            continue;
        }

        TealException(lua.GetGlobal("teal_skill") == Nz::LuaType_Table, "Lua: teal_skill isn't a table !");

        LuaArguments args = parseLua(lua);
        SkillData s(args);

        m_skills.addSkill(s.codename, s);
        NazaraDebug("Skill " + s.name + " loaded !");
    }
}

void Game::loadItems()
{
    Nz::Directory items { m_scriptPrefix + "items/" };
    items.SetPattern("*.lua");
    items.Open();

    while (items.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(items.GetResultPath()))
        {
            NazaraError("Error loading item " + items.GetResultName());
            continue;
        }

        TealException(lua.GetGlobal("teal_item") == Nz::LuaType_Table, "Lua: teal_item isn't a table !");

        Nz::String name = lua.CheckField<Nz::String>("name");
        Nz::String desc = lua.CheckField<Nz::String>("desc", "No description");
        unsigned  level = lua.CheckField<unsigned>("level", 1);

        Ndk::EntityHandle item = make_item(m_world, name, desc, level);

        TealException(lua.GetField("components") == Nz::LuaType_Table, "Lua: teal_item.components isn't a table !");

        for (int i { 1 };; ++i)
        {
            lua.PushInteger(i);

            if (lua.GetTable() == Nz::LuaType_Nil)
            {
                lua.Pop();
                break;
            }

            Nz::String componentType = lua.CheckField<Nz::String>("component");
            LuaArguments arguments = parseLua(lua);

            if (componentType == "AttackModifier")
                item->AddComponent<AttackModifierComponent>(arguments);

            if (componentType == "ResistanceModifier")
                item->AddComponent<ResistanceModifierComponent>(arguments);

            if (componentType == "Edible")
                item->AddComponent<Items::EdibleComponent>(arguments);

            if (componentType == "Equippable")
            {
                if (arguments.size() >= 1 && arguments[0].args.size() >= 3)
                    arguments[0].args[2].set<double>(m_skills.getSkillIndex(arguments[0].args[2].get<Nz::String>()));

                item->AddComponent<Items::EquippableComponent>(arguments);
            }

            if (componentType == "HPGain")
                item->AddComponent<Items::HPGainComponent>(arguments);

            if (componentType == "Resource")
                item->AddComponent<Items::ResourceComponent>(arguments);

            lua.Pop();
        }

        item->Enable(false);
        m_items.Insert(item);

        NazaraDebug("Item loaded - " + item->GetComponent<NameComponent>().name);
    }
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
    Ndk::InitializeSystem<FightSystem>();
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
                 Nz::TextureLibrary::Get(":/game/fight_tileset")->GetFilePath(), &m_tilesetCore, &m_fightTilesetCore);

    //mapComp.map->m_fightMode = true;
    //mapComp.map->update();

    activateMapEntities(MapDataLibrary::Get("0;0"));

    m_pather = std::make_shared<micropather::MicroPather>(mapComp.map.get(), Def::MAPX * Def::MAPY, 8);


    Nz::MaterialRef charMat = Nz::Material::New();
    charMat->Configure("Translucent2D");
    charMat->SetDiffuseMap(Nz::TextureLibrary::Get(":/game/char/villager"));

    Nz::SpriteRef charSprite = Nz::Sprite::New(charMat);
    charSprite->SetTextureRect({ 0u, 0u, 113u, 99u });


    CharacterData mainCharacData
    {
        { 113u, 99u }, charSprite, 15, { -25.f, -66.f }, { 1, 1 }, { 0, 0 }, 100u, AnimationComponent::OnMove, 
        Orientation::Down, {}, false, "me", "", { { Element::Fire, 100 } }
    };

    m_charac = make_character(m_world, mainCharacData);
    m_charac->GetComponent<Ndk::NodeComponent>().Move(0, 0, -1);
}

void Game::addSystems()
{
    m_world->AddSystem<AISystem>(m_pather);
    m_world->AddSystem<MovementSystem>();
    m_world->AddSystem<FightSystem>();
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
            showCharacteristics();
            break;
        }
    });
}

void Game::addWidgets()
{
    TealAssert(m_canvas, "Canvas null");
    m_canvas->SetPosition(10.f, static_cast<float>(Def::MAPYVIEWPORT) + 5);

    auto& eventHandler = m_window.GetEventHandler();

    Ndk::ButtonWidget* invButton = m_canvas->Add<Ndk::ButtonWidget>();
    TealAssert(invButton, "Inventory button null");

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
    m_world->GetSystem<FightSystem>().Enable(!enable);
}
