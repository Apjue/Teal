// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components.hpp>
#include <NDK/Systems/RenderSystem.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Graphics/ColorBackground.hpp>
#include <Nazara/Graphics/Material.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Core/File.hpp>
#include <Nazara/Lua.hpp>
#include <exception>
#include <algorithm>
#include <type_traits>
#include <iostream> // [TEST]
#include "components.hpp"
#include "systems.hpp"
#include "factory.hpp"
#include "data/characterdata.hpp"
#include "data/skilldata.hpp"
#include "util/assert.hpp"
#include "util/util.hpp"
#include "util/maputil.hpp"
#include "util/gfxutil.hpp"
#include "util/luaparser.hpp"
#include "util/gameutil.hpp"
#include "def/gamedef.hpp"
#include "def/typedef.hpp"
#include "def/uidef.hpp"
#include "def/layerdef.hpp"
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
    loadNazara();

    loadCharacters();
    loadTilesetCore();
    loadMaps();

    addIcon();
    addCam();

    loadMetaData();
    loadSkills();
    loadItems();

    addEntities();
    addSystems();

    initEventHandler();
    addWidgets();
    addPauseMenu();


    initializeGameUtility(m_charac);

    auto& mapComp = m_map->GetComponent<MapComponent>();
    initializeMapUtility(mapComp.map.get(), m_pather.get());
}

Ndk::EntityHandle Game::cloneCharacter(const Nz::String& codename)
{
    Ndk::EntityHandle e;
    auto it = std::find_if(m_characters.begin(), m_characters.end(),
                           [&codename] (const Ndk::EntityHandle& e) { return e->GetComponent<CloneComponent>().codename == codename; });

    if (it != m_characters.end())
    {
        e = (*it)->Clone();
        cloneRenderables(e);
    }

    return e;
}

void Game::showInventory(bool detail) // [TEST]
{
    auto& inv = m_charac->GetComponent<InventoryComponent>();
    unsigned counter {};

    for (auto& item : inv.items)
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
    std::cout << "Main character characteristics";
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
    TealException(Nz::File::Exists(m_scriptPrefix + "textures.lua"), "textures.lua not found !");

    Nz::LuaInstance lua;
    TealException(lua.ExecuteFromFile(m_scriptPrefix + "textures.lua"), "Lua: textures.lua loading failed !");
    TealException(lua.GetGlobal("teal_textures") == Nz::LuaType_Table, "Lua: teal_textures isn't a table !");

    for (int i { 1 };; ++i)
    {
        lua.PushInteger(i);

        if (lua.GetTable() != Nz::LuaType_Table)
        {
            lua.Pop();
            break;
        }

        lua.PushInteger(1);
        lua.GetTable();

        Nz::String id = lua.CheckString(-1);
        lua.Pop();


        lua.PushInteger(2);
        lua.GetTable();

        Nz::String filepath = lua.CheckString(-1);
        lua.Pop();

        Nz::TextureLibrary::Register(id, Nz::TextureManager::Get(m_imgPrefix + filepath));
        NazaraDebug("Texture " + id + " loaded !");

        lua.Pop();
    }

    NazaraDebug(" --- ");
}

void Game::loadTilesetCore()
{
    TealException(Nz::File::Exists(m_scriptPrefix + "tilesetcore.lua"), "tilesetcore.lua not found !");

    {
        Nz::LuaInstance lua;
        TealException(lua.ExecuteFromFile(m_scriptPrefix + "tilesetcore.lua"), "Lua: tilesetcore.lua loading failed !");
        TealException(lua.GetGlobal("teal_tilesetcore") == Nz::LuaType_Table, "Lua: teal_tilesetcore isn't a table !");

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
    }

    NazaraDebug("Loaded Tileset Core");
}

void Game::loadCharacters()
{
    Nz::Directory chars { m_scriptPrefix + "characters/" };
    chars.SetPattern("*.lua");
    chars.Open();

    while (chars.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(chars.GetResultPath()))
        {
            NazaraNotice("Error loading character " + chars.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }

        TealException(lua.GetGlobal("teal_character") == Nz::LuaType_Table, "Lua: teal_character isn't a table !");

        auto codename = lua.CheckField<Nz::String>("codename");
        auto sprite = lua.CheckField<Nz::String>("sprite", ":/game/unknown");
        auto maxFrames = lua.CheckField<unsigned>("maxanimframes", 1u, -1);
        auto maxHealth = lua.CheckField<unsigned>("maxhealth", 100u, -1);
        auto animType = AnimationComponent::stringToAnimState(lua.CheckField<Nz::String>("animtype", "onmove"));
        auto orientation = stringToOrientation(lua.CheckField<Nz::String>("orientation", "downleft"));
        auto blockTile = lua.CheckField<bool>("blocktile", false);
        auto name = lua.CheckField<Nz::String>("name", "Unnamed");
        auto description = lua.CheckField<Nz::String>("description", "Empty");
        auto level = lua.CheckField<unsigned>("level", 1u, -1);

        TealException(lua.GetField("size") == Nz::LuaType_Table, "Lua: teal_character.size isn't a table !");

        lua.PushInteger(1);
        TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.size.x isn't a number !");

        int sizex = toint(lua.CheckInteger(-1));
        TealException(sizex > 0, "Invalid size.x");
        lua.Pop();


        lua.PushInteger(2);
        TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.size.y isn't a number !");

        int sizey = toint(lua.CheckInteger(-1));
        TealException(sizey > 0, "Invalid size.y");
        lua.Pop();

        Nz::Vector2ui size = { tounsigned(sizex), tounsigned(sizey) };
        lua.Pop();


        TealException(lua.GetField("defgfxpos") == Nz::LuaType_Table, "Lua: teal_character.defgfxpos isn't a table !");

        lua.PushInteger(1);
        TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.defgfxpos.x isn't a number !");

        float defgfxposx = tofloat(lua.CheckInteger(-1));
        lua.Pop();


        lua.PushInteger(2);
        TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.defgfxpos.y isn't a number !");

        float defgfxposy = tofloat(lua.CheckInteger(-1));
        lua.Pop();

        Nz::Vector2f defgfxpos = { defgfxposx, defgfxposy };
        lua.Pop();


        TealException(lua.GetField("deflgcpos") == Nz::LuaType_Table, "Lua: teal_character.deflgcpos isn't a table !");

        lua.PushInteger(1);
        TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.deflgcpos.x isn't a number !");

        int deflgcposx = toint(lua.CheckInteger(-1));
        TealException(deflgcposx > 0, "Invalid deflgcpos.y");
        lua.Pop();


        lua.PushInteger(2);
        TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.deflgcpos.y isn't a number !");

        int deflgcposy = toint(lua.CheckInteger(-1));
        TealException(deflgcposy > 0, "Invalid deflgcpos.y");
        lua.Pop();

        Nz::Vector2ui deflgcpos = { tounsigned(deflgcposx), tounsigned(deflgcposy) };
        lua.Pop();


        CharacterData::RandomMovement random;

        if (lua.GetField("random") == Nz::LuaType_Table)
        {
            lua.PushInteger(1);

            if (lua.GetTable() == Nz::LuaType_Boolean && lua.CheckBoolean(-1))
            {
                lua.Pop();
                random = true;

                lua.PushInteger(2);

                if (lua.GetTable() == Nz::LuaType_Number)
                {
                    float movInterval = tofloat(lua.CheckNumber(-1));
                    TealException(movInterval > 0.f, "Invalid move interval");

                    random.movInterval = movInterval;
                    lua.Pop();

                    lua.PushInteger(3);

                    if (lua.GetTable() == Nz::LuaType_Number)
                    {
                        int nbTiles = toint(lua.CheckNumber(-1));
                        TealException(nbTiles > 0, "Invalid tiles number");

                        random.nbTiles = nbTiles;
                        lua.Pop();
                    }

                    lua.Pop();
                }

                lua.Pop();
            }

            lua.Pop();
        }

        lua.Pop();

        CharacterData::Elements attack;

        if (lua.GetField("attack") == Nz::LuaType_Table)
        {
            for (int i {};;)
            {
                lua.PushInteger(i);

                if (lua.GetTable() == Nz::LuaType_Table)
                {
                    lua.PushInteger(1);
                    lua.GetTable();

                    Nz::String element = lua.CheckString(-1);
                    lua.Pop();

                    lua.PushInteger(2);
                    lua.GetTable();

                    int modifier = toint(lua.CheckInteger(-1));
                    lua.Pop();

                    attack[stringToElement(element)] = modifier;
                }

                else
                {
                    lua.Pop();
                    break;
                }

                lua.Pop();
            }
        }

        lua.Pop();


        CharacterData::Elements res;

        if (lua.GetField("resistance") == Nz::LuaType_Table)
        {
            for (int i {};;)
            {
                lua.PushInteger(i);

                if (lua.GetTable() == Nz::LuaType_Table)
                {
                    lua.PushInteger(1);
                    lua.GetTable();

                    Nz::String element = lua.CheckString(-1);
                    lua.Pop();

                    lua.PushInteger(2);
                    lua.GetTable();

                    int modifier = toint(lua.CheckInteger(-1));
                    lua.Pop();

                    res[stringToElement(element)] = modifier;
                }

                else
                {
                    lua.Pop();
                    break;
                }

                lua.Pop();
            }
        }

        lua.Pop();

        CharacterData::Fight fight;

        if (lua.GetField("fight") == Nz::LuaType_Table)
        {
            fight.fight = true;
            fight.autoAttack = lua.CheckField<bool>("autoAttack", false);
            fight.movementPoints = lua.CheckField<unsigned>("movementPoints", 3, -1);
            fight.actionPoints = lua.CheckField<unsigned>("actionPoints", 6, -1);
        }

        lua.Pop();

        Nz::MaterialRef charMat = Nz::Material::New();
        charMat->Configure("Translucent2D");
        charMat->SetDiffuseMap(Nz::TextureLibrary::Get(sprite));

        Nz::SpriteRef charSprite = Nz::Sprite::New(charMat);
        charSprite->SetTextureRect({ 0u, 0u, size.x, size.y });

        CharacterData characterData
        {
            codename, size, charSprite, maxFrames, defgfxpos, deflgcpos, maxHealth, animType,
            orientation, random, blockTile, name, description, attack, res, level, fight
        };

        auto character = make_character(m_world, characterData);
        character->Enable(false);

        m_characters.Insert(character);
        NazaraDebug("Character " + name + " loaded ! (" + chars.GetResultName() + ")");
    }

    NazaraDebug(" --- ");
}

void Game::loadMaps()
{
    Nz::Directory maps { m_scriptPrefix + "maps/" };
    maps.SetPattern("*.lua");
    maps.Open();

    while (maps.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(maps.GetResultPath()))
        {
            NazaraNotice("Error loading map " + maps.GetResultName());
            NazaraNotice(lua.GetLastError());
            TealException(maps.GetResultName() != "map0_0.lua", "Lua: starting map fails to load");
            continue;
        }

        // Fun starts
        TealException(lua.GetGlobal("teal_map") == Nz::LuaType_Table, "Lua: teal_map isn't a table !");

        MapDataRef map = MapData::New();
        TileArray tiles;

        for (int i { 1 }; i <= Def::TileArraySize; ++i)
        {
            lua.PushInteger(i);
            lua.GetTable();

            TealException(lua.GetType(-1) == Nz::LuaType_Table, Nz::String { "Lua: teal_map." } +i + " isn't a table !");

            tiles[i - 1].textureId = lua.CheckField<Nz::String>("textureId");
            tiles[i - 1].fightTextureId = lua.CheckField<Nz::String>("fightTextureId");

            unsigned obstacle = lua.CheckField<unsigned>("obstacle");

            switch (obstacle)
            {
                case 1:
                    tiles[i - 1].addFlag("viewobstacle");
                    break;

                case 2:
                    tiles[i - 1].addFlag("blockobstacle");
                    break;
            }

            bool visible = lua.CheckField<bool>("visible");

            if (!visible)
                tiles[i - 1].addFlag("invisible");

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

            // get entities (codename)

            lua.Pop();
        }

        lua.Pop();

        Nz::String pos = lua.CheckField<Nz::String>("pos");
        map->setPosition(toVector(stringToMapXY(pos)));

        MapDataLibrary::Register(pos, deactivateMapEntities(map));
        NazaraDebug("Map " + maps.GetResultName() + " loaded at pos " + pos);
    }

    NazaraDebug(" --- ");
}

void Game::loadMetaData()
{
    m_states.addItem(PoisonnedState::getMetadataID(), { "Poisonned", "You are poisonned. Life sucks." });
    m_states.addItem(HealedState::getMetadataID(), { "Regeneration", "You are healed. Life is cool." });

    m_effects.addItem(PullEffect::getMetadataID(), { "Pull", "You are pulled. Someone wants the D." });
    m_effects.addItem(PushEffect::getMetadataID(), { "Push" });
}

void Game::loadSkills()
{
    Nz::Directory skills { m_scriptPrefix + "skills/" };
    skills.SetPattern("*.lua");
    skills.Open();

    while (skills.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(skills.GetResultPath()))
        {
            NazaraNotice("Error loading item " + skills.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }

        TealException(lua.GetGlobal("teal_skill") == Nz::LuaType_Table, "Lua: teal_skill isn't a table !");

        SkillData s(parseLua(lua));
        m_skills.addItem(s.codename, s);

        NazaraDebug("Skill " + s.name + " loaded ! (" + s.codename + ")");
    }

    NazaraDebug(" --- ");
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
            NazaraNotice("Error loading item " + items.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }

        TealException(lua.GetGlobal("teal_item") == Nz::LuaType_Table, "Lua: teal_item isn't a table !");

        Nz::String codename = lua.CheckField<Nz::String>("codename");
        Nz::String name = lua.CheckField<Nz::String>("name");
        Nz::String desc = lua.CheckField<Nz::String>("desc", "No description");
        unsigned  level = lua.CheckField<unsigned>("level");
        Nz::String icon = lua.CheckField<Nz::String>("icon");

        Ndk::EntityHandle item = make_logicalItem(m_world, codename, name, desc, level, Nz::TextureLibrary::Has(icon) ? Nz::TextureLibrary::Get(icon) : Nz::TextureLibrary::Get(":/game/unknown"));

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
            componentType = componentType.ToLower();

            if (componentType == "attackmodifier")
                item->AddComponent<AttackModifierComponent>(parseLua(lua));

            if (componentType == "resistancemodifier")
                item->AddComponent<ResistanceModifierComponent>(parseLua(lua));

            if (componentType == "edible")
                item->AddComponent<Items::EdibleComponent>(parseLua(lua));

            if (componentType == "equippable")
            {
                LuaArguments arguments = parseLua(lua);

                if (arguments.vars.size() >= 3)
                    arguments.vars[2].set<double>(m_skills.getItemIndex(arguments.vars[2].get<Nz::String>()));

                item->AddComponent<Items::EquippableComponent>(arguments);
            }

            if (componentType == "hpgain")
                item->AddComponent<Items::HPGainComponent>(parseLua(lua));

            if (componentType == "resource")
                item->AddComponent<Items::ResourceComponent>(parseLua(lua));

            lua.Pop();
        }

        item->Enable(false);
        m_items.Insert(item);

        NazaraDebug("Item loaded - " + item->GetComponent<NameComponent>().name);
    }

    NazaraDebug(" --- ");
}

void Game::loadNazara()
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
    Ndk::InitializeComponent<AttackModifierComponent>("atkmodif");
    Ndk::InitializeComponent<ResistanceModifierComponent>("resmodif");
    Ndk::InitializeComponent<DescriptionComponent>("desc");
    Ndk::InitializeComponent<CombatBehaviorComponent>("cbtbhv");
    Ndk::InitializeComponent<BlockTileComponent>("blcktile");
    Ndk::InitializeComponent<MonsterComponent>("monster");
    Ndk::InitializeComponent<EquipmentComponent>("equip");
    Ndk::InitializeComponent<LogicEntityIdComponent>("logicid");
    Ndk::InitializeComponent<IconComponent>("icon");
    Ndk::InitializeComponent<CloneComponent>("clone");
    Ndk::InitializeComponent<GraphicalEntitiesComponent>("gfxptr");
    Ndk::InitializeComponent<RenderablesStorageComponent>("fuckrtti");

    Ndk::InitializeComponent<Items::HPGainComponent>("hpgain");
    Ndk::InitializeComponent<Items::ItemComponent>("item");
    Ndk::InitializeComponent<Items::EquippableComponent>("iequip");
    Ndk::InitializeComponent<Items::EdibleComponent>("edible");
    Ndk::InitializeComponent<Items::ResourceComponent>("resource");

    // Systems
    Ndk::InitializeSystem<AISystem>();
    Ndk::InitializeSystem<MovementSystem>();
    Ndk::InitializeSystem<RandomMovementSystem>();
    Ndk::InitializeSystem<AnimationSystem>();
    Ndk::InitializeSystem<FightSystem>();
}

void Game::addIcon()
{
    Nz::Image iconImage;
    iconImage.LoadFromFile(Nz::TextureLibrary::Get(":/game/money")->GetFilePath());

    m_winIcon = Nz::Icon::New();
    m_winIcon->Create(iconImage);

    m_window.SetIcon(m_winIcon);
}

void Game::addCam()
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

void Game::addEntities() /// \todo Use lua (map's entities table)
{
    m_map = m_world->CreateEntity();

    auto& mapComp = m_map->AddComponent<MapComponent>();
    mapComp.init(MapDataLibrary::Get("0;0"), Nz::TextureLibrary::Get(":/game/tileset")->GetFilePath(),
                 Nz::TextureLibrary::Get(":/game/fight_tileset")->GetFilePath(), &m_tilesetCore, &m_fightTilesetCore);

    //mapComp.map->setFightMode(true);
    //mapComp.map->update();

    auto swordIt = std::find_if(m_items.begin(), m_items.end(), [] (const Ndk::EntityHandle& e)
                                                                   { return e->GetComponent<CloneComponent>().codename == "excalibur"; });

    if (swordIt != m_items.end())
    {
        auto logicEntity = (*swordIt)->Clone();
        logicEntity->AddComponent<PositionComponent>().xy = { 1, 2 };

        auto gfxEntity = make_mapItem(m_world, logicEntity, { 40, 40 }, { 12, -3 }, Def::MapItemsLayer);
        MapDataLibrary::Get("0;0")->getEntities().Insert(gfxEntity);
    }

    activateMapEntities(MapDataLibrary::Get("0;0"));
    m_pather = std::make_shared<micropather::MicroPather>(mapComp.map.get(), Def::ArrayMapX * Def::ArrayMapY, 8);

    m_charac = cloneCharacter("villager");
    m_charac->GetComponent<Ndk::NodeComponent>().Move(0, 0, -1);
    m_charac->GetComponent<PositionComponent>().xy = { 0, 1 };
    refreshGraphicsPos(m_charac);


    auto npc = cloneCharacter("villager");
    npc->GetComponent<PositionComponent>().xy = { 5, 5 };
    npc->GetComponent<NameComponent>().name = "The Wandering NPC";
    npc->AddComponent<RandomMovementComponent>(7.5f, 1);

    refreshGraphicsPos(npc);
    MapDataLibrary::Get("1;0")->getEntities().Insert(npc);
    deactivateMapEntities(MapDataLibrary::Get("1;0"));
}

void Game::addSystems()
{
    m_world->AddSystem<AISystem>(m_pather);
    m_world->AddSystem<MovementSystem>();
    m_world->AddSystem<FightSystem>();
    m_world->AddSystem<RandomMovementSystem>(m_map->GetComponent<MapComponent>().map);
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
            auto  tile = getTileFromGlobalCoords({ event.x, event.y });

            move.tile = tile;
            move.playerInitiated = true;
        }
    });

    m_keyPressEvent.Connect(eventHandler.OnKeyPressed,
                            [this] (const Nz::EventHandler*, const Nz::WindowEvent::KeyEvent& event)
    {
        if (event.code == Nz::Keyboard::Escape) // Pause menu
            enablePauseMenu(!m_paused);

        if (!m_paused)
            m_window.SetCursor(Nz::SystemCursor_Pointer);

        else
        {
            m_window.SetCursor(Nz::SystemCursor_Default);
            return;
        }

        switch (event.code)
        {
            case Nz::Keyboard::I: // Inventory
                showInventory(event.shift);
                break;

            case Nz::Keyboard::C: // Caracteristics
                showCharacteristics();
                break;

            case Nz::Keyboard::D: // Debug: Player pos
                auto& pos = m_charac->GetComponent<PositionComponent>();
                NazaraNotice("--- Debug ---");
                NazaraNotice(Nz::String { "Player position: " }
                             .Append(Nz::String::Number(pos.xy.x))
                             .Append(" ; ")
                             .Append(Nz::String::Number(pos.xy.y)));
                break;
        }
    });

    m_mouseMovedEvent.Connect(eventHandler.OnMouseMoved,
                              [this] (const Nz::EventHandler*, const Nz::WindowEvent::MouseMoveEvent& event)
    {
        if (m_mapViewport.Contains(event.x, event.y) && !m_paused)
            m_window.SetCursor(Nz::SystemCursor_Pointer);

        else
            m_window.SetCursor(Nz::SystemCursor_Default);
    });
}

void Game::addWidgets()
{
    TealAssert(m_canvas, "Canvas null");
    m_canvas->SetPosition(tofloat(Def::ButtonsMarginX), tofloat(Def::MapSizeY + Def::ButtonsMarginY));

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
    spriteBG->SetSize(tofloat(Def::WindowSizeX), tofloat(Def::WindowSizeY));

    gfxBG.Attach(spriteBG, Def::PauseMenuBackgroundLayer);

    // Pause Text
    m_pauseText = m_world->CreateEntity();
    m_pauseText->AddComponent<Ndk::NodeComponent>();

    auto& gfxPause = m_pauseText->AddComponent<Ndk::GraphicsComponent>();

    Nz::TextSpriteRef textPause = Nz::TextSprite::New();
    textPause->Update(Nz::SimpleTextDrawer::Draw("Pause menu\n"
                                                 "ESC = Toggle Pause Menu\n"
                                                 "(Shift) I = Inventory\n"
                                                 "C = Caracteristics", 20));

    gfxPause.Attach(textPause, Def::PauseMenuButtonsLayer);

    auto& nodePause = m_pauseText->GetComponent<Ndk::NodeComponent>();
    Nz::Boxf textBox = gfxPause.GetBoundingVolume().aabb;

    nodePause.SetPosition(Def::WindowSizeX / 2 - textBox.width / 2, 20);

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
