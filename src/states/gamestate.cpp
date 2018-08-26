// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/LuaAPI.hpp>
#include <Nazara/Platform/EventHandler.hpp>
#include <Nazara/Lua/LuaInstance.hpp>
#include "components/common/inventorycomponent.hpp"
#include "components/common/namecomponent.hpp"
#include "components/common/damagemodifiercomponent.hpp"
#include "components/common/inventorycomponent.hpp"
#include "components/common/mapcomponent.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/renderablesstoragecomponent.hpp"
#include "components/common/randommovementcomponent.hpp"
#include "components/common/movecomponent.hpp"
#include "widgets/spellbarwidget.hpp"
#include "util/cloneutil.hpp"
#include "util/maputil.hpp"
#include "util/gfxutil.hpp"
#include "def/layerdef.hpp"
#include "def/folderdef.hpp"
#include "def/uidef.hpp"
#include "factory.hpp"
#include "systems.hpp"
#include "states/gamestate.hpp"

GameState::GameState(GameData& gameData, const Nz::Vector2ui& mapArea)
    : m_world(gameData.world), m_window(*gameData.window), m_mapArea(mapArea), m_tilesetCore(gameData.tilesetCore),
    m_fightTilesetCore(gameData.fightTilesetCore), m_states(gameData.states), m_skills(gameData.skills), m_ais(gameData.ais), m_animations(gameData.animations),
    m_items(gameData.items), m_characters(gameData.characters)
{
    // Initialize scene (map & character)
    {
        Nz::LuaInstance lua;
        TealException(lua.ExecuteFromFile(Def::ScriptFolder + "character.lua"), "Lua: couldn't find character.lua file");
        TealException(lua.GetGlobal("teal_character") == Nz::LuaType_Table, "Lua: teal_character isn't a table !");

        // Map
        {
            Nz::Vector2i mapPos = lua.CheckField<Nz::Vector2i>("map", { 0, 0 }, -1);
            TealException(MapDataLibrary::Has(mapXYToString(mapPos.x, mapPos.y)), "Map doesn't exist!");

            m_map = m_world->CreateEntity();

            auto& mapComponent = m_map->AddComponent<MapComponent>();
            mapComponent.init(MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y)), Nz::TextureLibrary::Get(":/game/tileset")->GetFilePath(),
                              Nz::TextureLibrary::Get(":/game/fight_tileset")->GetFilePath(), &m_tilesetCore, &m_fightTilesetCore);

            m_map->Enable(false);
            deactivateMapEntities(MapDataLibrary::Get(mapXYToString(mapPos.x, mapPos.y)));
        }

        // Character
        {
            m_charac = cloneCharacter(m_characters, "villager");
            m_charac->Enable(false);

            m_charac->GetComponent<PositionComponent>().xy = lua.CheckField<AbsTile>("pos");
            m_charac->GetComponent<NameComponent>().name = lua.CheckField<Nz::String>("name");

            m_map->GetComponent<MapComponent>().map->getCurrentMap()->getEntities().Insert(m_charac); // that was one long line to type
        }
    }

    auto& mapComponent = m_map->GetComponent<MapComponent>();

    m_pather = std::make_shared<micropather::MicroPather>(mapComponent.map.get(), Def::ArrayMapX * Def::ArrayMapY, 8);
    initializeMapUtility(mapComponent.map.get(), m_pather.get(), m_charac);

    // Canvas Background
    Nz::SpriteRef canvasBackground = Nz::Sprite::New();
    canvasBackground->SetSize(Def::ButtonsSizeX, Def::ButtonsSizeY + Def::ButtonsMarginY * 2);
    canvasBackground->SetColor(Nz::Color { 40, 40, 40 });

    m_canvasBackgroundEntity = m_world->CreateEntity();
    m_canvasBackgroundEntity->AddComponent<Ndk::NodeComponent>().SetPosition(0.f, Def::MapSizeY);
    m_canvasBackgroundEntity->AddComponent<Ndk::GraphicsComponent>().Attach(canvasBackground);
    m_canvasBackgroundEntity->Enable(false);
}

GameState::~GameState()
{
    uninitializeMapUtility();
    m_pather.reset();
}

void GameState::Enter(Ndk::StateMachine& fsm)
{
    enableEntities();
    addSystems();
    initEventHandler();
    addWidgets();
}

void GameState::Leave(Ndk::StateMachine& fsm)
{
    removeSystems();
    removeWidgets();
    uninitEventHandler();
    disableEntities();
}

bool GameState::Update(Ndk::StateMachine& fsm, float elapsedTime)
{
    bool paused = !fsm.IsTopState(this);

    if (m_paused != paused)
    {
        m_paused = paused;
        handlePausedStateSwitch();
    }

    return true;
}

void GameState::handlePausedStateSwitch()
{
    m_world->GetSystem<AISystem>().Enable(!m_paused);
    m_world->GetSystem<MovementSystem>().Enable(!m_paused);
    m_world->GetSystem<AnimationSystem>().Enable(!m_paused);
    m_world->GetSystem<RandomMovementSystem>().Enable(!m_paused);
    m_world->GetSystem<FightSystem>().Enable(!m_paused);

    m_window.SetCursor(m_paused ? Nz::SystemCursor_Default : Nz::SystemCursor_Pointer);
}

#include <iostream>
void GameState::printInventory(bool detailled) /// Used for testing | todo: have a really UI for this
{
    auto& inv = m_charac->GetComponent<InventoryComponent>();
    unsigned counter {};

    for (auto& item : inv.items)
    {
        std::cout << "Item #" << counter;

        if (item->HasComponent<NameComponent>())
        {
            auto& name = item->GetComponent<NameComponent>();
            std::cout << " (Name: " << name.name << ')';
        }

        if (detailled)
        {
            if (item->HasComponent<DamageModifierComponent>())
            {
                auto& attack = item->GetComponent<DamageModifierComponent>().attack;
                std::cout << '\n';
                std::cout << "  Attack Modifier:\n";
                std::cout << "    Neutral: " << attack[Element::Neutral] << '\n';
                std::cout << "    Air: " << attack[Element::Air] << '\n';
                std::cout << "    Fire: " << attack[Element::Fire] << '\n';
                std::cout << "    Water: " << attack[Element::Water] << '\n';
                std::cout << "    Earth: " << attack[Element::Earth];

                auto& resistance = item->GetComponent<DamageModifierComponent>().resistance;
                std::cout << '\n';
                std::cout << "  Resistance Modifier:\n";
                std::cout << "    Neutral: " << resistance[Element::Neutral] << '\n';
                std::cout << "    Air: " << resistance[Element::Air] << '\n';
                std::cout << "    Fire: " << resistance[Element::Fire] << '\n';
                std::cout << "    Water: " << resistance[Element::Water] << '\n';
                std::cout << "    Earth: " << resistance[Element::Earth];
            }
        }

        std::cout << '\n';
        ++counter;
    }

    if (counter == 0)
        std::cout << "Inventory empty\n";

    std::cout << std::flush;
}

void GameState::printCharacteristics() /// Used for testing | todo: have a really UI for this
{
    std::cout << "Main character characteristics";
    if (m_charac->HasComponent<DamageModifierComponent>())
    {
        auto& attack = m_charac->GetComponent<DamageModifierComponent>().attack;
        std::cout << '\n';
        std::cout << "  Attack Modifier:\n";
        std::cout << "    Neutral: " << attack[Element::Neutral] << '\n';
        std::cout << "    Air: " << attack[Element::Air] << '\n';
        std::cout << "    Fire: " << attack[Element::Fire] << '\n';
        std::cout << "    Water: " << attack[Element::Water] << '\n';
        std::cout << "    Earth: " << attack[Element::Earth];

        auto& resistance = m_charac->GetComponent<DamageModifierComponent>().resistance;
        std::cout << '\n';
        std::cout << "  Resistance Modifier:\n";
        std::cout << "    Neutral: " << resistance[Element::Neutral] << '\n';
        std::cout << "    Air: " << resistance[Element::Air] << '\n';
        std::cout << "    Fire: " << resistance[Element::Fire] << '\n';
        std::cout << "    Water: " << resistance[Element::Water] << '\n';
        std::cout << "    Earth: " << resistance[Element::Earth];
    }

    std::cout << std::endl;
}

void GameState::enableEntities()
{
    m_map->Enable();
    activateMapEntities(m_map->GetComponent<MapComponent>().map->getCurrentMap());

    m_charac->Enable();
    refreshGraphicsPos(m_charac);
}

void GameState::addSystems()
{
    m_world->AddSystem<AISystem>(m_skills, m_ais, Def::FightAIUtilFile, m_pather, m_charac);
    m_world->AddSystem<MovementSystem>(m_charac);
    m_world->AddSystem<FightSystem>();
    m_world->AddSystem<RandomMovementSystem>(m_pather);
    m_world->AddSystem<AnimationSystem>();
}

void GameState::initEventHandler()
{
    auto& eventHandler = m_window.GetEventHandler();

    m_mouseButtonEvent.Connect(eventHandler.OnMouseButtonPressed, [this] (const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent& event)
    { // Lambda to move the player if the user clicked in the map
        if (!m_mapArea.Contains(event.x, event.y) || m_paused || event.button != Nz::Mouse::Left)
            return;

        Ndk::EntityList hoveredEntities = mapEntitiesHoveredByCursor({ event.x, event.y });

        if (!hoveredEntities.empty())
        {
            for (auto& entity : hoveredEntities)
            {
                if (isMonsterEntity(entity) && isFightableEntity(entity))
                {
                    std::cout << "todo: implement fight" << std::endl;
                }

                else /// todo: if (isNPCEntity(...))
                {
                    auto& move = m_charac->GetComponent<MoveComponent>();
                    AbsTile tile = getTileFromGlobalCoords({ event.x, event.y });

                    move.tile = tile;
                    move.playerInitiated = true;
                }
            }
        }

        else
        {
            auto& move = m_charac->GetComponent<MoveComponent>();
            AbsTile tile = getTileFromGlobalCoords({ event.x, event.y });

            move.tile = tile;
            move.playerInitiated = true;
        }
    });

    m_keyPressEvent.Connect(eventHandler.OnKeyPressed, [this] (const Nz::EventHandler*, const Nz::WindowEvent::KeyEvent& event)
    {
        switch (event.code)
        {
            case Nz::Keyboard::C: // Characteristics
                printCharacteristics();
                break;

            case Nz::Keyboard::I: // Inventory
                printInventory(event.shift);
                break;

            case Nz::Keyboard::U:
            {
                auto& renderSystem = m_world->GetSystem<Ndk::RenderSystem>();
                bool enable = !renderSystem.IsCullingEnabled();

                renderSystem.EnableCulling(enable);
                NazaraNotice(Nz::String { "Render System: Culling has been " } + (enable ? "enabled" : "disabled"));

                break;
            }

            case Nz::Keyboard::O: // ID
                NazaraNotice(Nz::String { "Character's Entity ID: " }.Append(Nz::String::Number(m_charac->GetId())));
                break;

            case Nz::Keyboard::P: // Position
                NazaraNotice(Nz::String { "Character's current position: " }.Append(m_charac->GetComponent<PositionComponent>().xy.ToString()));
                break;

            case Nz::Keyboard::R: // Raycasting test
            {
                if (m_raycastTiles.IsValid())
                    m_raycastTiles->Kill();

                if (event.shift) // Shift pressed? Tiles have been removed, stop
                {
                    NazaraNotice("Deleted raycasting tiles");
                    break;
                }

                NazaraNotice("Raycasting");
                std::vector<AbsTile> tiles = getVisibleTiles(m_charac->GetComponent<PositionComponent>().xy, 2);

                m_raycastTiles = m_world->CreateEntity();
                m_raycastTiles->AddComponent<Ndk::NodeComponent>();
                auto& gfx = m_raycastTiles->AddComponent<Ndk::GraphicsComponent>();

                for (auto& tilePos : tiles)
                {
                    Nz::SpriteRef tileSprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
                    tileSprite->SetTexture(Nz::TextureLibrary::Get(":/game/fight_tileset"));
                    tileSprite->SetSize(64, 32);
                    tileSprite->SetTextureRect({ 0, 0, 64, 32 });
                    auto aabb = getTileAABB(tilePos);

                    gfx.Attach(tileSprite, Nz::Matrix4f::Translate({ aabb.x, aabb.y, 0 }), Def::MapLayer + 1);
                }

                break;
            }

            case Nz::Keyboard::F5: // Refresh UI
                removeWidgets();
                addWidgets();

                NazaraNotice("UI refreshed");
                break;
        }
    });

    m_mouseMovedEvent.Connect(eventHandler.OnMouseMoved, [this] (const Nz::EventHandler*, const Nz::WindowEvent::MouseMoveEvent& event)
    {
        if (m_mapArea.Contains(event.x, event.y) && !m_paused)
            m_window.SetCursor(Nz::SystemCursor_Pointer);

        else
            m_window.SetCursor(Nz::SystemCursor_Default);
    });
}

void GameState::addWidgets()
{
    m_canvas = std::make_unique<Ndk::Canvas>(m_world->CreateHandle(), m_window.GetEventHandler(), m_window.GetCursorController().CreateHandle());
    m_canvas->SetPosition(float(Def::ButtonsPaddingX), float(Def::MapSizeY + Def::ButtonsMarginY));
    m_canvas->SetSize({ float(Def::ButtonsSizeX), float(Def::ButtonsSizeY) });

    m_canvasBackgroundEntity->Enable();

    // Canvas widgets
    auto& eventHandler = m_window.GetEventHandler();

    // Load config
    TealException(Nz::File::Exists(Def::ScriptFolder + "uiconfig.lua"), "uiconfig.lua not found !");

    Nz::LuaInstance lua;
    TealException(lua.ExecuteFromFile(Def::ScriptFolder + "uiconfig.lua"), "Lua: uiconfig.lua loading failed!");

    TealException(lua.GetGlobal("teal_ui_config") == Nz::LuaType_Table, "Lua: teal_ui_config isn't a table!");
    TealException(lua.GetField("buttons") == Nz::LuaType_Table, "Lua: teal_ui_config.buttons isn't a table!");


    TealException(lua.GetField("inventory") == Nz::LuaType_Table, "Lua: teal_ui_config.buttons.inventory isn't a table!");
    {

        Ndk::ButtonWidget* invButton = m_canvas->Add<Ndk::ButtonWidget>();

        invButton->SetTexture(Nz::TextureLibrary::Get(lua.CheckField<Nz::String>("texture")));
        invButton->SetHoverTexture(Nz::TextureLibrary::Get(lua.CheckField<Nz::String>("hover_texture")));
        invButton->SetPressTexture(Nz::TextureLibrary::Get(lua.CheckField<Nz::String>("press_texture")));

        invButton->SetPosition(lua.CheckField<Nz::Vector2f>("pos", {}, -1));
        invButton->SetSize(lua.CheckField<Nz::Vector2f>("size", invButton->GetSize()));

        TealException(lua.GetField("colors") == Nz::LuaType_Table, "Lua: teal_ui_config.buttons.inventory.colors isn't a table!");
        {
            auto colorPair = lua.CheckField<std::pair<Nz::Color, Nz::Color>>("default");
            auto hoverColorPair = lua.CheckField<std::pair<Nz::Color, Nz::Color>>("hover_color");
            auto pressColorPair = lua.CheckField<std::pair<Nz::Color, Nz::Color>>("press_color");

            invButton->SetColor(colorPair.first, colorPair.second);
            invButton->SetHoverColor(hoverColorPair.first, hoverColorPair.second);
            invButton->SetPressColor(pressColorPair.first, pressColorPair.second);
        }

        lua.Pop();

        m_invButtonEvent.Connect(invButton->OnButtonTrigger, [this] (const Ndk::ButtonWidget*)
        {
            if (!m_paused)
                printInventory();
        });
    }

    lua.Pop();


    TealException(lua.GetField("spell_bar") == Nz::LuaType_Table, "Lua: teal_ui_config.buttons.spell_bar isn't a table!");
    {
        SpellBarWidget* spellBar = m_canvas->Add<SpellBarWidget>();

        spellBar->SetPosition(lua.CheckField<Nz::Vector2f>("pos"));
        spellBar->setBarTexture(Nz::TextureLibrary::Get(lua.CheckField<Nz::String>("texture")));
        spellBar->setBarSize(lua.CheckField<Nz::Vector2f>("size", Nz::Vector2f(Nz::Vector2ui(spellBar->getBarTexture()->GetSize())), -1));

        spellBar->setBorderSize(lua.CheckField<Nz::Vector2ui>("border_size"));
        spellBar->setPadding(lua.CheckField<Nz::Vector2ui>("padding"));
        spellBar->setBoxSize(lua.CheckField<Nz::Vector2ui>("box_size"));
        spellBar->setBoxNumber(lua.CheckField<Nz::Vector2ui>("box_number"));

        spellBar->setDoubleClickInterval(lua.CheckField<Miliseconds>("double_click_interval", spellBar->getDoubleClickInterval(), -1));

        TealException(lua.GetField("arrows") == Nz::LuaType_Table, "Lua: teal_ui_config.buttons.spell_bar.arrows isn't a table!");
        {
            TealException(lua.GetField("down") == Nz::LuaType_Table, "Lua: teal_ui_config.buttons.spell_bar.arrows.down isn't a table!");
            {
                Ndk::ButtonWidget* downArrow = spellBar->getDownArrow();
                Nz::String texture = lua.CheckField<Nz::String>("texture");

                downArrow->SetTexture(Nz::TextureLibrary::Get(texture));
                downArrow->SetHoverTexture(Nz::TextureLibrary::Get(lua.CheckField<Nz::String>("hover_texture", texture, -1)));
                downArrow->SetPressTexture(Nz::TextureLibrary::Get(lua.CheckField<Nz::String>("press_texture", texture, -1)));

                downArrow->SetSize(lua.CheckField<Nz::Vector2f>("size", downArrow->GetSize()));
                downArrow->SetColor(Nz::Color::White, Nz::Color::White);
            }

            lua.Pop();

            TealException(lua.GetField("up") == Nz::LuaType_Table, "Lua: teal_ui_config.buttons.spell_bar.arrows.up isn't a table!");
            {
                Ndk::ButtonWidget* upArrow = spellBar->getUpArrow();
                Nz::String texture = lua.CheckField<Nz::String>("texture");

                upArrow->SetTexture(Nz::TextureLibrary::Get(texture));
                upArrow->SetHoverTexture(Nz::TextureLibrary::Get(lua.CheckField<Nz::String>("hover_texture", texture, -1)));
                upArrow->SetPressTexture(Nz::TextureLibrary::Get(lua.CheckField<Nz::String>("press_texture", texture, -1)));

                upArrow->SetSize(lua.CheckField<Nz::Vector2f>("size", upArrow->GetSize()));
                upArrow->SetColor(Nz::Color::White, Nz::Color::White);
            }

            lua.Pop();
        }

        lua.Pop();

        spellBar->ResizeToContent();
        spellBar->Show(false); // Just because it's buggy
    }

    lua.Pop();
}


void GameState::removeSystems()
{
    m_world->RemoveSystem<AISystem>();
    m_world->RemoveSystem<MovementSystem>();
    m_world->RemoveSystem<FightSystem>();
    m_world->RemoveSystem<RandomMovementSystem>();
    m_world->RemoveSystem<AnimationSystem>();
}

void GameState::disableEntities()
{
    m_map->Enable(false);
    deactivateMapEntities(m_map->GetComponent<MapComponent>().map->getCurrentMap());

    m_charac->Enable(false);
}

void GameState::uninitEventHandler()
{
    m_mouseButtonEvent.Disconnect();
    m_keyPressEvent.Disconnect();
    m_mouseMovedEvent.Disconnect();
}

void GameState::removeWidgets()
{
    m_invButtonEvent.Disconnect();

    m_canvas.reset();
    m_canvasBackgroundEntity->Enable(false);
}
