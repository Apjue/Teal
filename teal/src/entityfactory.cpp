// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components.hpp>
#include <vector>
#include "components.hpp"
#include "def/layerdef.hpp"
#include "util/gfxutil.hpp"
#include "util/util.hpp"
#include "entityfactory.hpp"

Ndk::EntityHandle makeLivingEntity(const Ndk::WorldHandle& w, const LivingEntityData& data)
{
    Ndk::EntityHandle e = w->CreateEntity();

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();
    gfx.Attach(data.sprite, Def::CharactersLayer);

    e->AddComponent<Ndk::NodeComponent>().SetPosition(data.offset.x, data.offset.y);
    e->AddComponent<GraphicsOffsetComponent>(data.offset);
    e->AddComponent<RenderablesStorageComponent>().sprites.push_back(data.sprite);
    e->AddComponent<AnimationComponent>().animList = data.animations;

    e->AddComponent<CloneComponent>().codename = data.codename;
    e->AddComponent<LifeComponent>().maxHp = data.maxHp;

    e->AddComponent<PositionComponent>(Nz::Vector2ui { 1u, 1u });
    e->AddComponent<MoveComponent>();
    e->AddComponent<PathComponent>();

    e->AddComponent<OrientationComponent>(data.orientation);

    if (data.fight.fight)
        e->AddComponent<FightComponent>(data.fight.autoAttack, data.fight.movementPoints, data.fight.actionPoints);

    e->AddComponent<StateComponent>();

    e->AddComponent<NameComponent>(data.name);
    e->AddComponent<DescriptionComponent>(data.description);

    e->AddComponent<DamageModifierComponent>().data = data.damageData;
    e->AddComponent<EquipmentComponent>();
    e->AddComponent<LevelComponent>(data.level);

    refreshGraphicsPos(e);
    return e;
}

Ndk::EntityHandle makeCharacter(const Ndk::WorldHandle& w, const CharacterData& data)
{
    Ndk::EntityHandle e = makeLivingEntity(w, data.livingEntityData);

    if (data.randomMovement.enabled)
        e->AddComponent<RandomMovementComponent>(data.randomMovement.movementInterval, data.randomMovement.range);

    if (data.blockTile)
        e->AddComponent<BlockTileComponent>();

    e->AddComponent<InventoryComponent>();

    return e;
}

Ndk::EntityHandle makeMonster(const Ndk::WorldHandle& w, const MonsterData& data)
{
    Ndk::EntityHandle e = makeLivingEntity(w, data.livingEntityData);

    e->AddComponent<MonsterComponent>().family = data.monsterFamily;

    return e;
}

Ndk::EntityHandle makeMonsterGroup(const Ndk::WorldHandle& w, float interval, unsigned range)
{
    Ndk::EntityHandle e = w->CreateEntity();

    e->AddComponent<PositionComponent>();
    e->AddComponent<RandomMovementComponent>(interval, range);
    e->AddComponent<MonsterGroupComponent>();

    return e;
}


Ndk::EntityHandle makeLogicalItem(const Ndk::WorldHandle& w, Nz::String codename, Nz::String name, Nz::String description,
                                  unsigned level, Nz::TextureRef icon, Nz::Vector2f mapOffset)
{
    Ndk::EntityHandle e = w->CreateEntity();

    e->AddComponent<ItemComponent>();
    e->AddComponent<CloneComponent>().codename = std::move(codename);
    e->AddComponent<NameComponent>().name = std::move(name);
    e->AddComponent<DescriptionComponent>().description = std::move(description);
    e->AddComponent<LevelComponent>().level = std::move(level);
    e->AddComponent<IconComponent>().icon = icon;
    e->AddComponent<GraphicsOffsetComponent>().offset = std::move(mapOffset);
    e->AddComponent<GraphicalEntitiesComponent>();

    return e;
}

Ndk::EntityHandle makeLogicalItem(const Ndk::WorldHandle& w, Nz::LuaInstance& lua)
{
    TealException(lua.GetGlobal("teal_item") == Nz::LuaType_Table, "Lua: teal_item isn't a table !");

    Nz::String codename = lua.CheckField<Nz::String>("codename");
    Nz::String name = lua.CheckField<Nz::String>("name");
    Nz::String description = lua.CheckField<Nz::String>("description", "No description");
    unsigned level = lua.CheckField<unsigned>("level");
    Nz::Vector2f mapOffset = lua.CheckField<Nz::Vector2f>("map_offset");

    Nz::String icon = lua.CheckField<Nz::String>("icon");
    Nz::TextureRef iconTexture = (Nz::TextureLibrary::Has(icon) ? Nz::TextureLibrary::Get(icon) : Nz::TextureLibrary::Get(":/game/unknown"));

    Ndk::EntityHandle e = makeLogicalItem(w, codename, name, description, level, iconTexture, mapOffset);

    TealException(lua.GetField("components") == Nz::LuaType_Table, "Lua: teal_item.components isn't a table !");

    if (lua.GetField(ConsumableComponent::componentName()) == Nz::LuaType_Table)
    {
        int index = -1;
        e->AddComponent<ConsumableComponent>(lua.Check<ConsumableComponent>(&index));
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

    if (lua.GetField(DamageModifierComponent::componentName()) == Nz::LuaType_Table)
    {
        int index = -1;
        e->AddComponent<DamageModifierComponent>(lua.Check<DamageModifierComponent>(&index));
    }
    lua.Pop();

    if (lua.GetField(StateComponent::componentName()) == Nz::LuaType_Table)
    {
        int index = -1;
        e->AddComponent<StateComponent>(lua.Check<StateComponent>(&index));
    }
    lua.Pop();

    return e;
}

Ndk::EntityHandle makeGraphicalItem(Ndk::EntityHandle e, const GraphicalItemData& data)
{
    const Ndk::EntityHandle& logicItem = data.logicItem;

    TealAssert(logicItem->HasComponent<ItemComponent>(), "Item isn't an actual item !");
    TealAssert(logicItem->GetComponent<IconComponent>().icon.IsValid() && logicItem->GetComponent<IconComponent>().icon->IsValid(), "Icon not valid");

    e->AddComponent<Ndk::NodeComponent>();

    auto& logic = e->AddComponent<LogicEntityIdComponent>();
    logic.logicEntity = logicItem;
    logic.itemType = data.itemType;

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();

    Nz::SpriteRef sprite = Nz::Sprite::New(Nz::Material::New("Translucent2D"));
    sprite->SetTexture(logicItem->GetComponent<IconComponent>().icon, false);
    sprite->SetSize(data.size);

    gfx.Attach(sprite, LogicEntityIdComponent::getRenderOrder(data.itemType)); // Using static function because the "logic" variable is maybe invalid
    e->AddComponent<RenderablesStorageComponent>().sprites.push_back(sprite);

    if (!logicItem->HasComponent<GraphicalEntitiesComponent>())
        logicItem->AddComponent<GraphicalEntitiesComponent>();

    logicItem->GetComponent<GraphicalEntitiesComponent>().entities.Insert(e);

    return e;
}

Ndk::EntityHandle makeGraphicalItem(const Ndk::WorldHandle& w, const GraphicalItemData& data)
{
    Ndk::EntityHandle e = w->CreateEntity();
    return makeGraphicalItem(e, data);
}

Ndk::EntityHandle makeGraphicalItem(const Ndk::WorldHandle& w, const GraphicalItemData& data, AbsTile pos)
{
    auto e = makeGraphicalItem(w, data);
    e->AddComponent<PositionComponent>().xy = pos;

    refreshGraphicsPos(e);
    return e;
}
