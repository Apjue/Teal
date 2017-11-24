// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Systems/RenderSystem.hpp>
#include <Nazara/Core/File.hpp>
#include <Nazara/Core/String.hpp>
#include <Nazara/Graphics/ColorBackground.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <Nazara/Lua/LuaInstance.hpp>
#include "components.hpp"
#include "systems.hpp"
#include "util/assert.hpp"
#include "util/gfxutil.hpp"
#include "def/folderdef.hpp"
#include "init/tealinit.hpp"

void initializeTeal(Ndk::World& world, Nz::RenderWindow& window)
{
    Detail::loadTextures();
    Detail::loadNazara();
    Detail::initSchemeUtility();
    Detail::loadTilesetCore();

    Detail::loadMetaData();
    Detail::loadSkills();
    Detail::loadAnimations();
    Detail::loadCharacters();
    Detail::loadItems();
    //Detail::loadMapObjects();
    Detail::loadMaps();

    Detail::addIcon(window);
    Detail::addCam(world, window);
}

namespace Detail
{

void loadTextures()
{
    TealException(Nz::File::Exists(Def::ScriptFolder + "textures.lua"), "textures.lua not found !");

    Nz::LuaInstance lua;
    TealException(lua.ExecuteFromFile(Def::ScriptFolder + "textures.lua"), "Lua: textures.lua loading failed !");
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

        Nz::TextureLibrary::Register(id, Nz::TextureManager::Get(Def::ImageFolder + filepath));
        NazaraDebug("Texture " + id + " loaded !");

        lua.Pop();
    }

    NazaraDebug(" --- ");
}

void loadNazara()
{
    // Components
    Ndk::InitializeComponent<RandomMovementComponent>("rdmov");
    Ndk::InitializeComponent<DefaultGraphicsPosComponent>("dfgfxpos");
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
    Ndk::InitializeComponent<Items::EquippableComponent>("canequip");
    Ndk::InitializeComponent<Items::EdibleComponent>("edible");
    Ndk::InitializeComponent<Items::ResourceComponent>("resource");

    // Systems
    Ndk::InitializeSystem<AISystem>();
    Ndk::InitializeSystem<MovementSystem>();
    Ndk::InitializeSystem<RandomMovementSystem>();
    Ndk::InitializeSystem<AnimationSystem>();
    Ndk::InitializeSystem<FightSystem>();
}

void initSchemeUtility()
{
    Nz::ImageRef scheme = Nz::Image::New();
    scheme->LoadFromFile(Nz::TextureLibrary::Get(":/game/scheme")->GetFilePath());

    initializeSchemeUtility(scheme);
}

void loadTilesetCore()
{

}


void loadMetaData()
{

}

void loadSkills()
{

}

void loadAnimations()
{

}

void loadCharacters()
{

}

void loadItems()
{

}

// void loadMapObjects()
// {
// 
// }

void loadMaps()
{

}

void addIcon(Nz::RenderWindow& window)
{

}

void addCam(Ndk::World& world, Nz::RenderWindow& window)
{
    auto& camera = world.CreateEntity();
    camera->AddComponent<Ndk::NodeComponent>();

    auto& cam = camera->AddComponent<Ndk::CameraComponent>();
    cam.SetProjectionType(Nz::ProjectionType_Orthogonal);
    cam.SetTarget(&window);

    auto& rendersys = world.GetSystem<Ndk::RenderSystem>();
    rendersys.SetGlobalUp(Nz::Vector3f::Down());
    rendersys.SetDefaultBackground(Nz::ColorBackground::New(Nz::Color::White));
}

}
