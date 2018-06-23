// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Lua/LuaBinding.hpp>
#include <NDK/LuaAPI.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Components/CameraComponent.hpp>
#include <NDK/Systems/RenderSystem.hpp>
#include <Nazara/Core/File.hpp>
#include <Nazara/Core/Directory.hpp>
#include <Nazara/Core/String.hpp>
#include <Nazara/Graphics/ColorBackground.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <Nazara/Lua/LuaInstance.hpp>
#include "components.hpp"
#include "systems.hpp"
#include "factory.hpp"
#include "util/assert.hpp"
#include "util/gfxutil.hpp"
#include "util/luaparser.hpp"
#include "util/fileutil.hpp"
#include "util/maputil.hpp"
#include "util/nzstlcompatibility.hpp"
#include "util/cloneutil.hpp"
#include "def/folderdef.hpp"
#include "data/states.hpp"
#include "data/effects.hpp"
#include "data/mapdata.hpp"
#include "data/characterdata.hpp"
#include "init/tealinit.hpp"

void initializeTeal(Ndk::World& world, Nz::RenderWindow& window, GameData& data)
{
    data.states = std::make_shared<StateMDStore>();
    data.skills = std::make_shared<SkillStore>();
    data.animations = std::make_shared<AnimationStore>();

    // Singletons
    DoubleStores<StateMetaData>::instance = data.states;
    DoubleStores<SkillData>::instance = data.skills;
    DoubleStores<AnimationData>::instance = data.animations;


    Detail::loadTextures();
    Detail::loadNazara();
    Detail::initSchemeUtility();
    Detail::loadTilesetCore(data.tilesetCore, data.fightTilesetCore);

    Detail::loadMetaData(*data.states);
    Detail::loadSkills(*data.skills);
    Detail::loadAnimations(*data.animations);
    Detail::loadCharacters(world, data.characters, *data.animations);
    Detail::loadItems(world, data.items, *data.skills);
    //Detail::loadMapObjects(data.mapObjects);
    Detail::loadMaps(world, data.characters, data.items);

    Detail::addIcon(window);
    Detail::addCam(world, window);
}

void uninitializeTeal(GameData& data)
{
    DoubleStores<StateMetaData>::instance.reset();
    DoubleStores<SkillData>::instance.reset();
    DoubleStores<AnimationData>::instance.reset();

    auto statemdptr = data.states;   // These pointers are on the stack and will be destructed at the end of the function
    auto skilldataptr = data.skills; // So objects managed by them (like ObjectRefs) won't be destroyed after the uninitialization of Nazara
    auto animdataptr = data.animations;

    data.states.reset();
    data.skills.reset();
    data.animations.reset();
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

        int index { -1 };
        std::pair<Nz::String, Nz::String> textureInfos = lua.Check<std::pair<Nz::String, Nz::String>>(&index);

        Nz::TextureLibrary::Register(textureInfos.first, Nz::TextureManager::Get(Def::ImageFolder + textureInfos.second));
        NazaraDebug("Texture " + textureInfos.first + " loaded !");

        lua.Pop();
    }

    NazaraDebug(" --- ");
}

void loadNazara()
{
    // Components
    Ndk::InitializeComponent<RandomMovementComponent>("rdmov");         //Ndk::LuaAPI::GetBinding()->BindComponent<RandomMovementComponent>("RandomMovementComponent");
    Ndk::InitializeComponent<DefaultGraphicsPosComponent>("dfgfxpos");  //Ndk::LuaAPI::GetBinding()->BindComponent<DefaultGraphicsPosComponent>("DefaultGraphicsPosComponent");
    Ndk::InitializeComponent<NameComponent>("name");                    //Ndk::LuaAPI::GetBinding()->BindComponent<NameComponent>("NameComponent");
    Ndk::InitializeComponent<LevelComponent>("level");                  Ndk::LuaAPI::GetBinding()->BindComponent<LevelComponent>("LevelComponent");
    Ndk::InitializeComponent<InventoryComponent>("inv");                //Ndk::LuaAPI::GetBinding()->BindComponent<InventoryComponent>("InventoryComponent");
    Ndk::InitializeComponent<OrientationComponent>("orient");           //Ndk::LuaAPI::GetBinding()->BindComponent<OrientationComponent>("OrientationComponent");
    Ndk::InitializeComponent<AnimationComponent>("anim");               //Ndk::LuaAPI::GetBinding()->BindComponent<AnimationComponent>("AnimationComponent");
    Ndk::InitializeComponent<PositionComponent>("pos");                 Ndk::LuaAPI::GetBinding()->BindComponent<PositionComponent>("PositionComponent");
    Ndk::InitializeComponent<MoveComponent>("move");                    //Ndk::LuaAPI::GetBinding()->BindComponent<MoveComponent>("MoveComponent");
    Ndk::InitializeComponent<PathComponent>("path");                    //Ndk::LuaAPI::GetBinding()->BindComponent<PathComponent>("PathComponent");
    Ndk::InitializeComponent<FightComponent>("fight");                  Ndk::LuaAPI::GetBinding()->BindComponent<FightComponent>("FightComponent");
    Ndk::InitializeComponent<LifeComponent>("life");                    Ndk::LuaAPI::GetBinding()->BindComponent<LifeComponent>("LifeComponent");
    Ndk::InitializeComponent<MapComponent>("map");                      //Ndk::LuaAPI::GetBinding()->BindComponent<MapComponent>("MapComponent");
    Ndk::InitializeComponent<DamageModifierComponent>("dmgmodif");      Ndk::LuaAPI::GetBinding()->BindComponent<DamageModifierComponent>("DamageModifierComponent");
    Ndk::InitializeComponent<DescriptionComponent>("desc");             //Ndk::LuaAPI::GetBinding()->BindComponent<DescriptionComponent>("DescriptionComponent");
    Ndk::InitializeComponent<CombatBehaviorComponent>("cbtbhv");        //Ndk::LuaAPI::GetBinding()->BindComponent<CombatBehaviorComponent>("CombatBehaviorComponent");
    Ndk::InitializeComponent<BlockTileComponent>("blcktile");           //Ndk::LuaAPI::GetBinding()->BindComponent<BlockTileComponent>("BlockTileComponent");
    Ndk::InitializeComponent<MonsterComponent>("monster");              //Ndk::LuaAPI::GetBinding()->BindComponent<MonsterComponent>("MonsterComponent");
    Ndk::InitializeComponent<EquipmentComponent>("equip");              //Ndk::LuaAPI::GetBinding()->BindComponent<EquipmentComponent>("EquipmentComponent");
    Ndk::InitializeComponent<LogicEntityIdComponent>("logicid");        //Ndk::LuaAPI::GetBinding()->BindComponent<LogicEntityIdComponent>("LogicEntityIdComponent");
    Ndk::InitializeComponent<IconComponent>("icon");                    //Ndk::LuaAPI::GetBinding()->BindComponent<IconComponent>("IconComponent");
    Ndk::InitializeComponent<CloneComponent>("clone");                  //Ndk::LuaAPI::GetBinding()->BindComponent<CloneComponent>("CloneComponent");
    Ndk::InitializeComponent<GraphicalEntitiesComponent>("gfxptr");     //Ndk::LuaAPI::GetBinding()->BindComponent<GraphicalEntitiesComponent>("GraphicalEntitiesComponent");
    Ndk::InitializeComponent<RenderablesStorageComponent>("fuckrtti");  //Ndk::LuaAPI::GetBinding()->BindComponent<RenderablesStorageComponent>("RenderablesStorageComponent");

    Ndk::InitializeComponent<HPGainComponent>("hpgain");                Ndk::LuaAPI::GetBinding()->BindComponent<HPGainComponent>("HPGainComponent");
    Ndk::InitializeComponent<ItemComponent>("item");                    //Ndk::LuaAPI::GetBinding()->BindComponent<ItemComponent>("ItemComponent");
    Ndk::InitializeComponent<EquippableComponent>("canequip");          Ndk::LuaAPI::GetBinding()->BindComponent<EquippableComponent>("EquippableComponent");
    Ndk::InitializeComponent<EdibleComponent>("edible");                Ndk::LuaAPI::GetBinding()->BindComponent<EdibleComponent>("EdibleComponent");
    Ndk::InitializeComponent<ResourceComponent>("resource");            Ndk::LuaAPI::GetBinding()->BindComponent<ResourceComponent>("ResourceComponent");

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

void loadTilesetCore(TilesetCore& tilesetCore, TilesetCore& fightTilesetCore)
{
    TealException(Nz::File::Exists(Def::ScriptFolder + "tilesetcore.lua"), "tilesetcore.lua not found !");

    {
        Nz::LuaInstance lua;
        TealException(lua.ExecuteFromFile(Def::ScriptFolder + "tilesetcore.lua"), "Lua: tilesetcore.lua loading failed !");
        TealException(lua.GetGlobal("teal_tileset_core") == Nz::LuaType_Table, "Lua: teal_tileset_core isn't a table !");

        for (int i { 1 };; ++i)
        {
            lua.PushInteger(i);

            if (lua.GetTable() != Nz::LuaType_Table)
            {
                lua.Pop();
                break;
            }

            tilesetCore.add(lua.CheckField<unsigned>("index"), lua.CheckField<Nz::String>("name"));
            lua.Pop();
        }
    }

    {
        Nz::LuaInstance lua;
        TealException(lua.ExecuteFromFile(Def::ScriptFolder + "tilesetcore.lua"), "Lua: tilesetcore.lua loading failed !");
        TealException(lua.GetGlobal("teal_fight_tileset_core") == Nz::LuaType_Table, "Lua: teal_fight_tileset_core isn't a table !");

        for (int i { 1 };; ++i)
        {
            lua.PushInteger(i);

            if (lua.GetTable() != Nz::LuaType_Table)
            {
                lua.Pop();
                break;
            }

            fightTilesetCore.add(lua.CheckField<unsigned>("index"), lua.CheckField<Nz::String>("name"));
            lua.Pop();
        }
    }

    NazaraDebug("Loaded Tileset Core");
}


void loadMetaData(StateMDStore& states)
{
    states.addItem(PoisonnedState::getMetadataID(), { "Poisonned", "You are poisonned. Life sucks." });
    states.addItem(HealedState::getMetadataID(), { "Regeneration", "You are healed. Life is cool." });
}

void loadSkills(SkillStore& skills)
{
    Nz::Directory skillsDirectory { Def::SkillFolder };
    skillsDirectory.SetPattern("*.lua");
    skillsDirectory.Open();

    while (skillsDirectory.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(skillsDirectory.GetResultPath()))
        {
            NazaraNotice("Error loading skill " + skillsDirectory.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }

        SkillData s = lua.CheckGlobal<SkillData>("teal_skill");
        skills.addItem(s.codename, s);

        NazaraDebug("Skill " + s.displayName + " loaded ! (" + s.codename + ")");
    }

    NazaraDebug(" --- ");
}

void loadFightAIs(AICore& ais)
{
    static constexpr unsigned wildcardTagPower { 1 }; // * + *
    static constexpr unsigned familyTagPower { wildcardTagPower + 1 }; // Family + *
    static constexpr unsigned preciseTagPower { familyTagPower + 1 }; // Family + Monster Name


    {   // Default AI
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(Def::FightAIUtilFile))
        {
            NazaraNotice("Error loading default AI");
            NazaraNotice(lua.GetLastError());
        }

        else
            ais.addTag(std::make_pair("*", "*"), std::make_pair(Def::FightAIUtilFile, wildcardTagPower));
    }


    Nz::Directory aisDirectory { Def::FightAIFolder };
    aisDirectory.SetPattern("*.lua");
    aisDirectory.Open();

    while (aisDirectory.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(aisDirectory.GetResultPath()))
        {
            NazaraNotice("Error loading AI " + aisDirectory.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }

        Nz::String aiName = lua.CheckGlobal<Nz::String>("fight_ai_name");
        Nz::String monsterFamily = lua.CheckGlobal<Nz::String>("fight_ai_monster_family");
        Nz::String monsterName = lua.CheckGlobal<Nz::String>("fight_ai_monster_name");

        if (monsterFamily == "*") // I already have a default AI, thanks but no thanks
            continue;

        if (monsterName == "*")
            ais.addTag(std::make_pair(monsterFamily, monsterName), std::make_pair(aisDirectory.GetResultPath(), familyTagPower));

        ais.addTag(std::make_pair(monsterFamily, monsterName), std::make_pair(aisDirectory.GetResultPath(), preciseTagPower));

        NazaraDebug("AI " + aiName + " loaded ! (" + aisDirectory.GetResultPath() + ")");
    }

    NazaraDebug(" --- ");
}

void loadAnimations(AnimationStore& animations)
{
    Nz::Directory animationsDirectory { Def::AnimationFolder };
    animationsDirectory.SetPattern("*.lua");
    animationsDirectory.Open();

    while (animationsDirectory.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(animationsDirectory.GetResultPath()))
        {
            NazaraNotice("Error loading animation " + animationsDirectory.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }

        AnimationData animation = lua.CheckGlobal<AnimationData>("teal_animation");
        Nz::String codename = removeFileNameExtension(animationsDirectory.GetResultName());

        animations.addItem(codename, animation);
        NazaraDebug("Animation " + codename + " loaded ! (" + animationsDirectory.GetResultName() + ")");
    }

    NazaraDebug(" --- ");
}

void loadCharacters(Ndk::World& world, Ndk::EntityList& characters, AnimationStore& animations)
{
    Nz::Directory charactersDirectory { Def::CharacterFolder };
    charactersDirectory.SetPattern("*.lua");
    charactersDirectory.Open();

    while (charactersDirectory.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(charactersDirectory.GetResultPath()))
        {
            NazaraNotice("Error loading character " + charactersDirectory.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }

        CharacterData characterData = lua.CheckGlobal<CharacterData>("teal_character");
        characterData.codename = removeFileNameExtension(charactersDirectory.GetResultName());

        auto character = makeCharacter(world.CreateHandle(), characterData);
        character->Enable(false);

        characters.Insert(character);
        NazaraDebug("Character " + characterData.name + " loaded ! (" + characterData.codename + ")");
    }

    NazaraDebug(" --- ");
}

void loadItems(Ndk::World& world, Ndk::EntityList& items, const SkillStore& skills)
{
    Nz::Directory itemsDirectory { Def::ItemFolder };
    itemsDirectory.SetPattern("*.lua");
    itemsDirectory.Open();

    while (itemsDirectory.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(itemsDirectory.GetResultPath()))
        {
            NazaraNotice("Error loading item " + itemsDirectory.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }
        
        Ndk::EntityHandle item = makeLogicalItem(world.CreateHandle(), lua);

        item->Enable(false);
        items.Insert(item);

        NazaraDebug("Item loaded - " + item->GetComponent<NameComponent>().name);
    }

    NazaraDebug(" --- ");
}

// void loadMapObjects()
// {
// 
// }

void loadMaps(Ndk::World& world, const Ndk::EntityList& characters, const Ndk::EntityList& items)
{
    Nz::Directory maps { Def::MapFolder };
    maps.SetPattern("*.lua");
    maps.Open();

    while (maps.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(maps.GetResultPath()))
        {
            NazaraNotice("Error loading map " + maps.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }

        // Fun starts
        TealException(lua.GetGlobal("teal_map") == Nz::LuaType_Table, "Lua: teal_map isn't a table !");

        MapDataRef map;
        LuaImplQueryArg(lua, -1, &map, Nz::TypeTag<MapDataRef>());
        const Nz::Vector2i& mapPos = map->getPosition();

        TealException(lua.GetField("entities") == Nz::LuaType_Table, "Lua: teal_map.entities isn't a table !");

        for (int i { 1 };; ++i)
        {
            lua.PushInteger(i);

            if (lua.GetTable() != Nz::LuaType_Table)
            {
                lua.Pop();
                break;
            }

            Nz::String codename = lua.CheckField<Nz::String>("codename");
            Nz::String type = lua.CheckField<Nz::String>("type").ToLower();
            AbsTile pos = lua.CheckField<AbsTile>("pos");

            if (type == "character")
            {
                Ndk::EntityHandle e = cloneCharacter(characters, codename);

                if (e.IsValid())
                {
                    e->Enable(false);

                    e->GetComponent<PositionComponent>().xy = pos;
                    refreshGraphicsPos(e);

                    map->getEntities().Insert(e);
                }
            }

            else if (type == "item")
            {
                Ndk::EntityHandle e = cloneItem(items, codename);

                if (e.IsValid())
                {
                    Nz::Vector2f size = lua.CheckField<Nz::Vector2f>("size");
                    Nz::Vector2f offset = lua.CheckField<Nz::Vector2f>("offset");

                    auto gfxEntity = makeGraphicalItem(world.CreateHandle(), { e, pos, size, offset, LogicEntityIdComponent::GroundItem });
                    gfxEntity->Enable(false);

                    refreshGraphicsPos(gfxEntity);
                    map->getEntities().Insert(gfxEntity);
                }
            }

            else
            {
                NazaraNotice(Nz::String { "Invalid type for entity " }.Append(codename).Append(" in map ")
                             .Append(mapXYToString(mapPos.x, mapPos.y)).Append(" [with type = \"").Append(type).Append("\"]"));
            }

            lua.Pop();
        }

        lua.Pop();

        MapDataLibrary::Register(mapXYToString(mapPos.x, mapPos.y), deactivateMapEntities(map));
        NazaraDebug("Map " + maps.GetResultName() + " loaded at pos " + mapXYToString(mapPos.x, mapPos.y));
    }

    NazaraDebug(" --- ");
}

void addIcon(Nz::RenderWindow& window)
{
    Nz::Image iconImage;
    iconImage.LoadFromFile(Nz::TextureLibrary::Get(":/game/money")->GetFilePath());

    Nz::IconRef winIcon = Nz::Icon::New();
    winIcon->Create(iconImage);

    window.SetIcon(winIcon);
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
