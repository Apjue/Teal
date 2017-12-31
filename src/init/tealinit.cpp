// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

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
    Detail::loadTextures();
    Detail::loadNazara();
    Detail::initSchemeUtility();
    Detail::loadTilesetCore(data.tilesetCore, data.fightTilesetCore);

    Detail::loadMetaData(data.states, data.effects);
    Detail::loadSkills(data.skills);
    Detail::loadAnimations(data.animations);
    Detail::loadCharacters(world, data.characters, data.animations);
    Detail::loadItems(world, data.items, data.skills);
    //Detail::loadMapObjects(data.mapObjects);
    Detail::loadMaps(data.characters, data.items);

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

void loadTilesetCore(TilesetCore& tilesetCore, TilesetCore& fightTilesetCore)
{
    TealException(Nz::File::Exists(Def::ScriptFolder + "tilesetcore.lua"), "tilesetcore.lua not found !");

    {
        Nz::LuaInstance lua;
        TealException(lua.ExecuteFromFile(Def::ScriptFolder + "tilesetcore.lua"), "Lua: tilesetcore.lua loading failed !");
        TealException(lua.GetGlobal("teal_tilesetcore") == Nz::LuaType_Table, "Lua: teal_tilesetcore isn't a table !");

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

        lua.GetGlobal("teal_fighttilesetcore");
        TealException(lua.GetType(-1) == Nz::LuaType_Table, "Lua: teal_fighttilesetcore isn't a table !");

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


void loadMetaData(StateMDStore& states, EffectMDStore& effects)
{
    states.addItem(PoisonnedState::getMetadataID(), { "Poisonned", "You are poisonned. Life sucks." });
    states.addItem(HealedState::getMetadataID(), { "Regeneration", "You are healed. Life is cool." });

    effects.addItem(PullEffect::getMetadataID(), { "Pull" });
    effects.addItem(PushEffect::getMetadataID(), { "Push" });
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

        TealException(lua.GetGlobal("teal_skill") == Nz::LuaType_Table, "Lua: teal_skill isn't a table !");

        SkillData s(parseLua(lua));
        skills.addItem(s.codename, s);

        NazaraDebug("Skill " + s.name + " loaded ! (" + s.codename + ")");
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

        TealException(lua.GetGlobal("teal_animation") == Nz::LuaType_Table, "Lua: teal_animation isn't a table !");
        Nz::String codename = removeFileNameExtension(animationsDirectory.GetResultName());

        auto animType = AnimationData::stringToAnimationType(lua.CheckField<Nz::String>("type"));
        Nz::TextureRef texture = Nz::TextureLibrary::Get(lua.CheckField<Nz::String>("texture", ":/game/unknown"));

        TealException(lua.GetField("size") == Nz::LuaType_Table, "Lua: teal_animation.size isn't a table !");
        Nz::Vector2ui size;
        {
            lua.PushInteger(1);
            TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_animation.size.x isn't a number !");

            int sizex = int(lua.CheckInteger(-1));
            TealException(sizex > 0, "Invalid size.x");
            lua.Pop();


            lua.PushInteger(2);
            TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_animation.size.y isn't a number !");

            int sizey = int(lua.CheckInteger(-1));
            TealException(sizey > 0, "Invalid size.y");
            lua.Pop();

            size = { unsigned(sizex), unsigned(sizey) };
        }
        lua.Pop();


        TealException(lua.GetField("defgfxpos") == Nz::LuaType_Table, "Lua: teal_animation.defgfxpos isn't a table !");
        Nz::Vector2f defgfxpos;
        {
            lua.PushInteger(1);
            {
                TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_animation.defgfxpos.x isn't a number !");
                defgfxpos.x = float(lua.CheckInteger(-1));
            }
            lua.Pop();

            lua.PushInteger(2);
            {
                TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_animation.defgfxpos.y isn't a number !");
                defgfxpos.y = float(lua.CheckInteger(-1));
            }
            lua.Pop();
        }
        lua.Pop();

        AnimationData animation { animType, size, texture, defgfxpos };
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

        TealException(lua.GetGlobal("teal_character") == Nz::LuaType_Table, "Lua: teal_character isn't a table !");
        Nz::String codename = removeFileNameExtension(charactersDirectory.GetResultName());

        auto texture = lua.CheckField<Nz::String>("texture", ":/game/unknown");
        auto maxHealth = lua.CheckField<unsigned>("maxhealth", 100u, -1);
        auto orientation = stringToOrientation(lua.CheckField<Nz::String>("orientation", "downleft"));
        auto blockTile = lua.CheckField<bool>("blocktile", false);
        auto name = lua.CheckField<Nz::String>("name", "Unnamed");
        auto description = lua.CheckField<Nz::String>("description", "Empty");
        auto level = lua.CheckField<unsigned>("level", 1u, -1);


        TealException(lua.GetField("size") == Nz::LuaType_Table, "Lua: teal_character.size isn't a table !");
        Nz::Vector2ui size;
        {
            lua.PushInteger(1);
            {
                TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.size.x isn't a number !");

                auto sizex = lua.CheckInteger(-1);
                TealException(sizex > 0, "Invalid size.x");
                size.x = unsigned(sizex);
            }
            lua.Pop();


            lua.PushInteger(2);
            {
                TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.size.y isn't a number !");

                auto sizey = lua.CheckInteger(-1);
                TealException(sizey > 0, "Invalid size.y");
                size.y = unsigned(sizey);
            }
            lua.Pop();
        }
        lua.Pop();


        TealException(lua.GetField("deflgcpos") == Nz::LuaType_Table, "Lua: teal_character.deflgcpos isn't a table !");
        Nz::Vector2ui deflgcpos;
        {
            lua.PushInteger(1);
            TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.deflgcpos.x isn't a number !");

            int deflgcposx = int(lua.CheckInteger(-1));
            TealException(deflgcposx > 0, "Invalid deflgcpos.y");
            lua.Pop();


            lua.PushInteger(2);
            TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.deflgcpos.y isn't a number !");

            int deflgcposy = int(lua.CheckInteger(-1));
            TealException(deflgcposy > 0, "Invalid deflgcpos.y");
            lua.Pop();

            deflgcpos = { unsigned(deflgcposx), unsigned(deflgcposy) };
        }
        lua.Pop();

        TealException(lua.GetField("defgfxpos") == Nz::LuaType_Table, "Lua: teal_character.defgfxpos isn't a table !");
        Nz::Vector2f defgfxpos;
        {
            lua.PushInteger(1);
            {
                TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.defgfxpos.x isn't a number !");
                defgfxpos.x = float(lua.CheckInteger(-1));
            }
            lua.Pop();


            lua.PushInteger(2);
            {
                TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_character.defgfxpos.y isn't a number !");
                defgfxpos.y = float(lua.CheckInteger(-1));
            }
            lua.Pop();
        }
        lua.Pop();


        TealException(lua.GetField("animations") == Nz::LuaType_Table, "Lua: teal_character.animations isn't a table !");

        LuaArguments animArgs = parseLua(lua);
        AnimationComponent::AnimationList characterAnimations;
        std::size_t defaultAnimation = AnimationComponent::InvalidAnimationID;

        for (auto& animVariant : animArgs.vars)
        {
            if (!animVariant.is<Nz::String>())
            {
                NazaraError("Animation codename: String expected");
                continue;
            }

            Nz::String& animName = animVariant.get<Nz::String>();

            if (!animations.hasItem(animName))
            {
                NazaraError("Animation codename not found: " + animName);
                continue;
            }

            AnimationData anim = animations.getItem(animName); // Yup, copy ctor
            characterAnimations.emplace_back(std::move(anim));
        }

        if (!characterAnimations.empty())
            defaultAnimation = 0; // Todo: change this

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
                    float movInterval = float(lua.CheckNumber(-1));
                    TealException(movInterval > 0.f, "Invalid move interval");

                    random.movInterval = movInterval;
                    lua.Pop();

                    lua.PushInteger(3);

                    if (lua.GetTable() == Nz::LuaType_Number)
                    {
                        int nbTiles = int(lua.CheckNumber(-1));
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

                    int modifier = int(lua.CheckInteger(-1));
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

                    int modifier = int(lua.CheckInteger(-1));
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
        charMat->SetDiffuseMap(Nz::TextureLibrary::Get(texture));

        Nz::SpriteRef charSprite = Nz::Sprite::New(charMat);
        charSprite->SetMaterial(charMat, false);
        charSprite->SetTextureRect({ 0u, 0u, size.x, size.y });
        charSprite->SetSize(float(size.x), float(size.y));

        CharacterData characterData
        {
            codename, charSprite, defgfxpos, characterAnimations, defaultAnimation, maxHealth, orientation, random, blockTile, name, description, attack, res, level, fight
        };

        auto character = make_character(world.CreateHandle(), characterData);
        character->Enable(false);

        characters.Insert(character);
        NazaraDebug("Character " + name + " loaded ! (" + charactersDirectory.GetResultName() + ")");
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

        TealException(lua.GetGlobal("teal_item") == Nz::LuaType_Table, "Lua: teal_item isn't a table !");

        Nz::String codename = lua.CheckField<Nz::String>("codename");
        Nz::String name = lua.CheckField<Nz::String>("name");
        Nz::String desc = lua.CheckField<Nz::String>("desc", "No description");
        unsigned  level = lua.CheckField<unsigned>("level");
        Nz::String icon = lua.CheckField<Nz::String>("icon");

        Ndk::EntityHandle item = make_logicalItem(world.CreateHandle(), codename, name, desc, level,
                                                  Nz::TextureLibrary::Has(icon) ? Nz::TextureLibrary::Get(icon) : Nz::TextureLibrary::Get(":/game/unknown"));

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
                    arguments.vars[2].set<double>(skills.getItemIndex(arguments.vars[2].get<Nz::String>()));

                item->AddComponent<Items::EquippableComponent>(arguments);
            }

            if (componentType == "hpgain")
                item->AddComponent<Items::HPGainComponent>(parseLua(lua));

            if (componentType == "resource")
                item->AddComponent<Items::ResourceComponent>(parseLua(lua));

            lua.Pop();
        }

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

void loadMaps(const Ndk::EntityList& characters, const Ndk::EntityList& items)
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
        Nz::String mapPos = lua.CheckField<Nz::String>("pos");

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

            Nz::String codename = lua.CheckField<Nz::String>("codename");
            Nz::String type = lua.CheckField<Nz::String>("type").ToLower();

            TealException(lua.GetField("pos") == Nz::LuaType_Table, "Lua: teal_map.entities.pos isn't a table !");

            lua.PushInteger(1);
            TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_map.entities.pos.x isn't a number !");

            int posx = int(lua.CheckInteger(-1));
            TealException(posx > 0, "Invalid pos.y");
            lua.Pop();


            lua.PushInteger(2);
            TealException(lua.GetTable() == Nz::LuaType_Number, "Lua: teal_map.entities.pos.y isn't a number !");

            int posy = int(lua.CheckInteger(-1));
            TealException(posy > 0, "Invalid pos.y");
            lua.Pop();

            Nz::Vector2ui pos = { unsigned(posx), unsigned(posy) };
            lua.Pop();


            if (type == "character" || type == "item")
            {
                Ndk::EntityHandle e = (type == "character" ? cloneCharacter(characters, codename) : cloneItem(items, codename));

                if (e.IsValid())
                {
                    e->Enable(false);
                    map->getEntities().Insert(e);
                }
            }

            else
            {
                NazaraNotice(Nz::String { "Invalid type for entity " }.Append(codename).Append(" in map ")
                             .Append(mapPos).Append(" [with type = \"").Append(type).Append("\"]"));
            }

            lua.Pop();
        }

        lua.Pop();

        map->setPosition(toVector(stringToMapXY(mapPos)));
        MapDataLibrary::Register(mapPos, deactivateMapEntities(map));

        NazaraDebug("Map " + maps.GetResultName() + " loaded at pos " + mapPos);
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
