// Copyright (C) 2019 Samy Bensaid
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
#include <array>
#include "components.hpp"
#include "systems.hpp"
#include "entityfactory.hpp"
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

void initializeTeal(GameData& data)
{
    data.states     = std::make_shared<StateMDStore>();
    data.skills     = std::make_shared<SkillStore>();
    data.animations = std::make_shared<AnimationStore>();

    // Singletons
    DoubleStores<StateMetaData>::instance = data.states;
    DoubleStores<SkillData>    ::instance = data.skills;
    DoubleStores<AnimationData>::instance = data.animations;


    TealInitDetail::loadTextures();
    TealInitDetail::loadNazara();
    TealInitDetail::initSchemeUtility();
    TealInitDetail::loadTilesetCore(data.tilesetCore, data.fightTilesetCore);

    TealInitDetail::loadStatesMetaData(*data.states);
    TealInitDetail::loadSkills(*data.skills);
    TealInitDetail::loadAnimations(*data.animations);
    TealInitDetail::loadCharacters(data.world, data.characters);
    TealInitDetail::loadMonsters(data.world, data.monsters);
    TealInitDetail::loadItems(data.world, data.items, *data.skills);
    //Detail::loadMapObjects(data.mapObjects);
    TealInitDetail::loadMaps(data.world, data.characters, data.items);

    TealInitDetail::addIcon(*data.window);
    TealInitDetail::addCam(data.world, *data.window);
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

namespace TealInitDetail
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
        NazaraNotice("Texture " + textureInfos.first + " loaded !");

        lua.Pop();
    }

    // Ensure required textures are here
    static std::array<Nz::String, 3> requiredTextures
    {
        ":/game/unknown", ":/game/tileset", ":/game/fight_tileset"
    };

    for (auto& textureName : requiredTextures)
        TealException(Nz::TextureLibrary::Has(textureName), Nz::String { "Required texture " }.Append(textureName).Append(" isn't here"));

    NazaraNotice(" --- ");
}

void loadNazara()
{
    // Components
    Ndk::InitializeComponent<RandomMovementComponent>("rdmov");         //Ndk::LuaAPI::GetBinding()->BindComponent<RandomMovementComponent>("RandomMovementComponent");
    Ndk::InitializeComponent<GraphicsOffsetComponent>("gfxoffst");      //Ndk::LuaAPI::GetBinding()->BindComponent<GraphicsOffsetComponent>("GraphicsOffsetComponent");
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
    Ndk::InitializeComponent<BlockTileComponent>("blcktile");           //Ndk::LuaAPI::GetBinding()->BindComponent<BlockTileComponent>("BlockTileComponent");
    Ndk::InitializeComponent<MonsterComponent>("monster");              //Ndk::LuaAPI::GetBinding()->BindComponent<MonsterComponent>("MonsterComponent");
    Ndk::InitializeComponent<EquipmentComponent>("equip");              //Ndk::LuaAPI::GetBinding()->BindComponent<EquipmentComponent>("EquipmentComponent");
    Ndk::InitializeComponent<LogicEntityIdComponent>("logicid");        //Ndk::LuaAPI::GetBinding()->BindComponent<LogicEntityIdComponent>("LogicEntityIdComponent");
    Ndk::InitializeComponent<IconComponent>("icon");                    //Ndk::LuaAPI::GetBinding()->BindComponent<IconComponent>("IconComponent");
    Ndk::InitializeComponent<CloneComponent>("clone");                  //Ndk::LuaAPI::GetBinding()->BindComponent<CloneComponent>("CloneComponent");
    Ndk::InitializeComponent<GraphicalEntitiesComponent>("gfxptr");     //Ndk::LuaAPI::GetBinding()->BindComponent<GraphicalEntitiesComponent>("GraphicalEntitiesComponent");
    Ndk::InitializeComponent<RenderablesStorageComponent>("fuckrtti");  //Ndk::LuaAPI::GetBinding()->BindComponent<RenderablesStorageComponent>("RenderablesStorageComponent");
    Ndk::InitializeComponent<StateComponent>("feelsbad");               Ndk::LuaAPI::GetBinding()->BindComponent<StateComponent>("StateComponent");

    Ndk::InitializeComponent<HPGainComponent>("hpgain");                Ndk::LuaAPI::GetBinding()->BindComponent<HPGainComponent>("HPGainComponent");
    Ndk::InitializeComponent<ItemComponent>("item");                    //Ndk::LuaAPI::GetBinding()->BindComponent<ItemComponent>("ItemComponent");
    Ndk::InitializeComponent<EquippableComponent>("canequip");          Ndk::LuaAPI::GetBinding()->BindComponent<EquippableComponent>("EquippableComponent");
    Ndk::InitializeComponent<ConsumableComponent>("consum");            Ndk::LuaAPI::GetBinding()->BindComponent<ConsumableComponent>("ConsumableComponent");
    Ndk::InitializeComponent<ResourceComponent>("resource");            Ndk::LuaAPI::GetBinding()->BindComponent<ResourceComponent>("ResourceComponent");

    // Systems
    Ndk::InitializeSystem<AISystem>();
    Ndk::InitializeSystem<MovementSystem>();
    Ndk::InitializeSystem<RandomMovementSystem>();
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

    NazaraNotice("Loaded Tileset Core");
}


void loadStatesMetaData(StateMDStore& states)
{
    Nz::Directory statesDirectory { Def::StateFolder };
    statesDirectory.SetPattern("*.lua");
    statesDirectory.Open();

    while (statesDirectory.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(statesDirectory.GetResultPath()))
        {
            NazaraNotice("Error loading state " + statesDirectory.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }

        StateMetaData s = lua.CheckGlobal<StateMetaData>("teal_state_metadata");
        states.addItem(s.codename, s);

        NazaraNotice("State " + s.name + " loaded ! (" + s.codename + ")");
    }

    NazaraNotice(" --- ");
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

        NazaraNotice("Skill " + s.displayName + " loaded ! (" + s.codename + ")");
    }

    NazaraNotice(" --- ");
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

        ais.addTag(std::make_pair(monsterFamily, monsterName), std::make_pair(aisDirectory.GetResultPath(), (monsterName == "*" ? familyTagPower : preciseTagPower)));
        NazaraNotice("AI " + aiName + " loaded ! (" + aisDirectory.GetResultPath() + ")");
    }

    NazaraNotice(" --- ");
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
        NazaraNotice("Animation " + codename + " loaded ! (" + animationsDirectory.GetResultName() + ")");
    }

    NazaraNotice(" --- ");
}

void loadCharacters(Ndk::WorldHandle world, Ndk::EntityList& characters)
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
        characterData.livingEntityData.codename = removeFileNameExtension(charactersDirectory.GetResultName());

        auto character = makeCharacter(world, characterData);
        character->Enable(false);

        characters.Insert(character);
        NazaraNotice("Character " + characterData.livingEntityData.name + " loaded ! (" + characterData.livingEntityData.codename + ")");
    }

    NazaraNotice(" --- ");
}

void loadMonsters(Ndk::WorldHandle world, Ndk::EntityList& monsters)
{
    Nz::Directory monstersDirectory { Def::MonsterFolder };
    monstersDirectory.SetPattern("*.lua");
    monstersDirectory.Open();

    while (monstersDirectory.NextResult())
    {
        Nz::LuaInstance lua;

        if (!lua.ExecuteFromFile(monstersDirectory.GetResultPath()))
        {
            NazaraNotice("Error loading character " + monstersDirectory.GetResultName());
            NazaraNotice(lua.GetLastError());
            continue;
        }

        MonsterData monsterData = lua.CheckGlobal<MonsterData>("teal_monster");
        monsterData.livingEntityData.codename = removeFileNameExtension(monstersDirectory.GetResultName());

        auto monster = makeMonster(world, monsterData);
        monster->Enable(false);

        monsters.Insert(monster);
        NazaraNotice("Monster " + monsterData.livingEntityData.name + " loaded ! (" + monsterData.livingEntityData.codename + ")");
    }

    NazaraNotice(" --- ");
}

void loadItems(Ndk::WorldHandle world, Ndk::EntityList& items, const SkillStore& skills)
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
        
        Ndk::EntityHandle item = makeLogicalItem(world, lua);

        item->Enable(false);
        items.Insert(item);

        NazaraNotice("Item loaded - " + item->GetComponent<NameComponent>().name);
    }

    NazaraNotice(" --- ");
}

// void loadMapObjects()
// {
// 
// }

void loadMaps(Ndk::WorldHandle world, const Ndk::EntityList& characters, const Ndk::EntityList& items)
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


                    auto randomMovement = lua.CheckField<CharacterData::RandomMovement>("random_movement");

                    if (randomMovement.enabled)
                    {
                        if (e->HasComponent<RandomMovementComponent>())
                            e->RemoveComponent<RandomMovementComponent>();

                        e->AddComponent<RandomMovementComponent>(randomMovement.movementInterval, randomMovement.range);
                    }

                    auto& orientComp = e->GetComponent<OrientationComponent>();
                    Orientation orientation = stringToOrientation(lua.CheckField<Nz::String>("orientation", orientationToString(orientComp.orientation)));
                    orientComp.orientation = orientation;

                    map->getEntities().Insert(e);
                }
            }

            else if (type == "monstergroup")
            {
                //...
            }

            else if (type == "item")
            {
                Ndk::EntityHandle e = cloneItem(items, codename);

                if (e.IsValid())
                {
                    auto gfxEntity = makeGraphicalItem(world, { e, { Def::MapItemDefaultSize, Def::MapItemDefaultSize }, LogicEntityIdComponent::GroundItem }, pos);

                    gfxEntity->Enable(false);
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
        NazaraNotice("Map " + maps.GetResultName() + " loaded at pos " + mapXYToString(mapPos.x, mapPos.y));
    }

    NazaraNotice(" --- ");
}

void addIcon(Nz::RenderWindow& window)
{
    Nz::Image iconImage;
    iconImage.LoadFromFile(Nz::TextureLibrary::Get(":/game/money")->GetFilePath());

    Nz::IconRef winIcon = Nz::Icon::New();
    winIcon->Create(iconImage);

    window.SetIcon(winIcon);
}

void addCam(Ndk::WorldHandle world, Nz::RenderWindow& window)
{
    auto& camera = world->CreateEntity();
    camera->AddComponent<Ndk::NodeComponent>();

    auto& cam = camera->AddComponent<Ndk::CameraComponent>();
    cam.SetProjectionType(Nz::ProjectionType_Orthogonal);
    cam.SetTarget(&window);

    auto& rendersys = world->GetSystem<Ndk::RenderSystem>();
    rendersys.SetGlobalUp(Nz::Vector3f::Down());
    rendersys.SetDefaultBackground(Nz::ColorBackground::New(Nz::Color::White));
}

}
