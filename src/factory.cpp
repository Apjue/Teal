// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components.hpp>
#include <vector>
#include "components.hpp"
#include "def/layerdef.hpp"
#include "util/gfxutil.hpp"
#include "util/util.hpp"
#include "factory.hpp"

Ndk::EntityHandle makeCharacter(const Ndk::WorldHandle& w, const CharacterData& data)
{
    Ndk::EntityHandle e = w->CreateEntity();

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();
    gfx.Attach(data.sprite, Def::CharactersLayer);

    e->AddComponent<Ndk::NodeComponent>().SetPosition(data.offset.x, data.offset.y);
    e->AddComponent<DefaultGraphicsPosComponent>(data.offset);
    e->AddComponent<RenderablesStorageComponent>().sprites.push_back(data.sprite);
    e->AddComponent<AnimationComponent>().animList = data.animations;

    e->AddComponent<CloneComponent>().codename = data.codename;
    e->AddComponent<LifeComponent>().maxHp = data.maxHp;

    e->AddComponent<PositionComponent>(Nz::Vector2ui { 1u, 1u });
    e->AddComponent<MoveComponent>();
    e->AddComponent<PathComponent>();
    e->AddComponent<InventoryComponent>();

    e->AddComponent<OrientationComponent>(data.orientation);

    if (data.randomMovement.randomMovement)
        e->AddComponent<RandomMovementComponent>(data.randomMovement.movementInterval, data.randomMovement.range);

    if (data.fight.fight)
        e->AddComponent<FightComponent>(data.fight.autoAttack, data.fight.movementPoints, data.fight.actionPoints);

    e->AddComponent<NameComponent>(data.name);
    e->AddComponent<DescriptionComponent>(data.description);
    e->AddComponent<BlockTileComponent>().blockTile = data.blockTile;

    auto& dmg = e->AddComponent<DamageModifierComponent>();
    dmg.attack = data.attack;
    dmg.resistance = data.resistance;

    e->AddComponent<EquipmentComponent>();
    e->AddComponent<CombatBehaviorComponent>();
    e->AddComponent<LevelComponent>(data.level);

    refreshGraphicsPos(e);
    return e;
}

Ndk::EntityHandle makeLogicalItem(const Ndk::WorldHandle& w, const Nz::String& codename, const Nz::String& name, 
                                   const Nz::String& description, unsigned level, Nz::TextureRef icon)
{
    Ndk::EntityHandle e = w->CreateEntity();

    e->AddComponent<ItemComponent>();
    e->AddComponent<CloneComponent>().codename = codename;
    e->AddComponent<NameComponent>().name = name;
    e->AddComponent<DescriptionComponent>().description = description;
    e->AddComponent<LevelComponent>(level);
    e->AddComponent<IconComponent>().icon = icon;
    e->AddComponent<GraphicalEntitiesComponent>();

    return e;
}

Ndk::EntityHandle makeLogicalItem(const Ndk::WorldHandle& w, Nz::LuaInstance& lua)
{
    TealException(lua.GetGlobal("teal_item") == Nz::LuaType_Table, "Lua: teal_item isn't a table !");
    Ndk::EntityHandle e = w->CreateEntity();

    e->AddComponent<ItemComponent>();
    e->AddComponent<CloneComponent>().codename = lua.CheckField<Nz::String>("codename");
    e->AddComponent<NameComponent>().name = lua.CheckField<Nz::String>("name");
    e->AddComponent<DescriptionComponent>().description = lua.CheckField<Nz::String>("description", "No description");
    e->AddComponent<LevelComponent>().level = lua.CheckField<unsigned>("level");
    e->AddComponent<GraphicalEntitiesComponent>();

    Nz::String icon = lua.CheckField<Nz::String>("icon");
    e->AddComponent<IconComponent>().icon = (Nz::TextureLibrary::Has(icon) ? Nz::TextureLibrary::Get(icon) : Nz::TextureLibrary::Get(":/game/unknown"));


    TealException(lua.GetField("components") == Nz::LuaType_Table, "Lua: teal_item.components isn't a table !");

    if (lua.GetField(EdibleComponent::componentName()) == Nz::LuaType_Table)
    {
        int index = -1;
        e->AddComponent<EdibleComponent>(lua.Check<EdibleComponent>(&index));
    }
    lua.Pop();

    if (lua.GetField(EquippableComponent::componentName()) == Nz::LuaType_Table)
    {
        int index = -1;
        e->AddComponent<EquippableComponent>(lua.Check<EquippableComponent>(&index));
    }
    lua.Pop();

    if (lua.GetField(HPGainComponent::componentName()) == Nz::LuaType_Table)
    {
        int index = -1;
        e->AddComponent<HPGainComponent>(lua.Check<HPGainComponent>(&index));
    }
    lua.Pop();

    if (lua.GetField(ResourceComponent::componentName()) == Nz::LuaType_Table)
    {
        int index = -1;
        e->AddComponent<ResourceComponent>(lua.Check<ResourceComponent>(&index));
    }
    lua.Pop();

    return e;
}

Ndk::EntityHandle makeGraphicalItem(const Ndk::WorldHandle& w, const GraphicalItemData& data)
{
    const Ndk::EntityHandle& logicItem = data.logicItem;

    TealAssert(logicItem->HasComponent<ItemComponent>(), "Item isn't an actual item !");
    TealAssert(logicItem->GetComponent<IconComponent>().icon.IsValid() && logicItem->GetComponent<IconComponent>().icon->IsValid(), "Icon not valid");

    Ndk::EntityHandle e = w->CreateEntity();

    e->AddComponent<Ndk::NodeComponent>();
    e->AddComponent<PositionComponent>().xy = data.pos;

    auto& logic = e->AddComponent<LogicEntityIdComponent>();
    logic.logicEntity = logicItem;
    logic.itemType = data.itemType;

    e->AddComponent<DefaultGraphicsPosComponent>(data.offset);

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();

    Nz::SpriteRef sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
    sprite->SetTexture(logicItem->GetComponent<IconComponent>().icon, false);
    sprite->SetSize(data.size);

    gfx.Attach(sprite, LogicEntityIdComponent::getRenderOrder(data.itemType)); // Using static function because the "logic" variable is maybe invalid
    e->AddComponent<RenderablesStorageComponent>().sprites.push_back(sprite);

    if (!logicItem->HasComponent<GraphicalEntitiesComponent>())
        logicItem->AddComponent<GraphicalEntitiesComponent>();

    logicItem->GetComponent<GraphicalEntitiesComponent>().entities.Insert(e);
    refreshGraphicsPos(e);

    return e;
}
