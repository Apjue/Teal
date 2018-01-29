// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components.hpp>
#include "components.hpp"
#include "def/layerdef.hpp"
#include "util/gfxutil.hpp"
#include "util/util.hpp"
#include "factory.hpp"

Ndk::EntityHandle makeCharacter(const Ndk::WorldHandle& w, const CharacterData& data)
{
    Ndk::EntityHandle e = w->CreateEntity();
    data.sprite->SetOrigin({ 0.f, 0.f, 0.f }); // Nazara bug

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();
    gfx.Attach(data.sprite, Def::CharactersLayer);

    e->AddComponent<Ndk::NodeComponent>().SetPosition(data.defG.x, data.defG.y);
    e->AddComponent<DefaultGraphicsPosComponent>(data.defG);
    e->AddComponent<RenderablesStorageComponent>().sprites.push_back(data.sprite);

    auto& anims = e->AddComponent<AnimationComponent>();
    anims.animList = data.animations;
    anims.currentAnimation = data.defaultAnimation;

    e->AddComponent<CloneComponent>(data.codename);
    e->AddComponent<LifeComponent>(data.maxhp);

    e->AddComponent<PositionComponent>(Nz::Vector2ui { 1u, 1u });
    e->AddComponent<MoveComponent>();
    e->AddComponent<PathComponent>();
    e->AddComponent<InventoryComponent>();

    e->AddComponent<OrientationComponent>(data.o);

    if (data.rdMov.randomMovement)
        e->AddComponent<RandomMovementComponent>(data.rdMov.movInterval, data.rdMov.range);

    if (data.fight.fight)
        e->AddComponent<FightComponent>(data.fight.autoAttack, data.fight.movementPoints, data.fight.actionPoints);

    e->AddComponent<NameComponent>(data.name);
    e->AddComponent<DescriptionComponent>(data.desc);
    e->AddComponent<BlockTileComponent>().blockTile = data.blockTile;

    auto& atk = e->AddComponent<AttackModifierComponent>();
    atk.data = data.atk;

    auto& res = e->AddComponent<ResistanceModifierComponent>();
    res.data = data.res;

    e->AddComponent<EquipmentComponent>();
    e->AddComponent<CombatBehaviorComponent>();
    e->AddComponent<LevelComponent>(data.level);

    refreshGraphicsPos(e);
    return e;
}

Ndk::EntityHandle makeLogicalItem(const Ndk::WorldHandle& w, const Nz::String& codename, const Nz::String& name, 
                                   const Nz::String& desc, unsigned level, Nz::TextureRef icon)
{
    Ndk::EntityHandle e = w->CreateEntity();

    e->AddComponent<Items::ItemComponent>();
    e->AddComponent<CloneComponent>(codename);
    e->AddComponent<NameComponent>().name = name;
    e->AddComponent<DescriptionComponent>().description = desc;
    e->AddComponent<LevelComponent>(level);
    e->AddComponent<IconComponent>().icon = icon;
    e->AddComponent<GraphicalEntitiesComponent>();

    return e;
}

Ndk::EntityHandle makeGraphicalItem(const Ndk::WorldHandle& w, const GraphicalItemData& data)
{
    const Ndk::EntityHandle& logicItem = data.logicItem;

    TealAssert(logicItem->HasComponent<Items::ItemComponent>(), "Item isn't an actual item !");
    TealAssert(logicItem->GetComponent<IconComponent>().icon.IsValid() && logicItem->GetComponent<IconComponent>().icon->IsValid(), "Icon not valid");

    Ndk::EntityHandle e = w->CreateEntity();

    e->AddComponent<Ndk::NodeComponent>();

    auto& logic = e->AddComponent<LogicEntityIdComponent>();
    logic.logicEntity = logicItem;
    logic.itemType = data.itemType;

    e->AddComponent<DefaultGraphicsPosComponent>(data.defGfxPos);

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();

    Nz::SpriteRef sprite = Nz::Sprite::New();
    sprite->SetTexture(logicItem->GetComponent<IconComponent>().icon, false);
    sprite->SetSize(data.size);

    gfx.Attach(sprite, LogicEntityIdComponent::getRenderOrder(data.itemType)); // Using static function because the "logic" variable is maybe invalid
    e->AddComponent<RenderablesStorageComponent>().sprites.push_back(sprite);

    if (!logicItem->HasComponent<GraphicalEntitiesComponent>())
        logicItem->AddComponent<GraphicalEntitiesComponent>();

    logicItem->GetComponent<GraphicalEntitiesComponent>().entities.Insert(e);
    refreshGraphicsPos(logicItem, e);

    return e;
}
