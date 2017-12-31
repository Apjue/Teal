// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components.hpp>
#include "components.hpp"
#include "def/layerdef.hpp"
#include "util/gfxutil.hpp"
#include "util/util.hpp"
#include "factory.hpp"

Ndk::EntityHandle make_character(const Ndk::WorldHandle& w, const CharacterData& infos)
{
    Ndk::EntityHandle e = w->CreateEntity();
    infos.sprite->SetOrigin({ 0.f, 0.f, 0.f }); // Nazara bug

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();
    gfx.Attach(infos.sprite, Def::CharactersLayer);

    e->AddComponent<Ndk::NodeComponent>().SetPosition(infos.defG.x, infos.defG.y);
    e->AddComponent<DefaultGraphicsPosComponent>(infos.defG);
    e->AddComponent<RenderablesStorageComponent>().sprites.push_back(infos.sprite);

    auto& anims = e->AddComponent<AnimationComponent>();
    anims.animList = infos.animations;
    anims.currentAnimation = infos.defaultAnimation;

    e->AddComponent<CloneComponent>(infos.codename);
    e->AddComponent<LifeComponent>(infos.maxhp);

    e->AddComponent<PositionComponent>(Nz::Vector2ui { 1u, 1u });
    e->AddComponent<MoveComponent>();
    e->AddComponent<PathComponent>();
    e->AddComponent<InventoryComponent>();

    e->AddComponent<OrientationComponent>(infos.o);

    if (infos.rdMov.randomMovement)
        e->AddComponent<RandomMovementComponent>(infos.rdMov.movInterval, infos.rdMov.nbTiles);

    if (infos.fight.fight)
        e->AddComponent<FightComponent>(infos.fight.autoAttack, infos.fight.movementPoints, infos.fight.actionPoints);

    e->AddComponent<NameComponent>(infos.name);
    e->AddComponent<DescriptionComponent>(infos.desc);
    e->AddComponent<BlockTileComponent>().blockTile = infos.blockTile;

    auto& atk = e->AddComponent<AttackModifierComponent>();
    atk.data = infos.atk;

    auto& res = e->AddComponent<ResistanceModifierComponent>();
    res.data = infos.res;

    e->AddComponent<EquipmentComponent>();
    e->AddComponent<CombatBehaviorComponent>();
    e->AddComponent<LevelComponent>(infos.level);

    refreshGraphicsPos(e);
    return e;
}

Ndk::EntityHandle make_logicalItem(const Ndk::WorldHandle& w, const Nz::String& codename, const Nz::String& name, 
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

extern Ndk::EntityHandle make_mapItem(const Ndk::WorldHandle& w, const Ndk::EntityHandle& logicItem, const Nz::Vector2f& size, const Nz::Vector2f& defGfxPos, int renderOrder)
{
    auto e = make_graphicalItem(w, logicItem, size, defGfxPos, renderOrder);
    logicItem->GetComponent<GraphicalEntitiesComponent>().entities.Insert(e);

    return e;
}

Ndk::EntityHandle make_graphicalItem(const Ndk::WorldHandle& w, const Ndk::EntityHandle& logicItem, const Nz::Vector2f& size, const Nz::Vector2f& defGfxPos, int renderOrder)
{
    TealAssert(logicItem->HasComponent<Items::ItemComponent>(), "Item isn't an actual item !");
    TealAssert(logicItem->GetComponent<IconComponent>().icon.IsValid() && logicItem->GetComponent<IconComponent>().icon->IsValid(), "Icon not valid");

    Ndk::EntityHandle e = w->CreateEntity();

    e->AddComponent<Ndk::NodeComponent>();
    e->AddComponent<LogicEntityIdComponent>().logicEntity = logicItem;
    e->AddComponent<DefaultGraphicsPosComponent>(defGfxPos);

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();

    Nz::SpriteRef sprite = Nz::Sprite::New();
    sprite->SetTexture(logicItem->GetComponent<IconComponent>().icon, false);
    sprite->SetSize(size);

    gfx.Attach(sprite, renderOrder);
    e->AddComponent<RenderablesStorageComponent>().sprites.push_back(sprite);

    refreshGraphicsPos(logicItem, e);

    return e;
}
