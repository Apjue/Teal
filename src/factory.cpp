// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "factory.hpp"

Ndk::EntityHandle make_character(const Ndk::WorldHandle& w, const CharacterData& infos)
{
    Ndk::EntityHandle e = w->CreateEntity();

    infos.sprite->SetSize(static_cast<float>(infos.imgsize.x), static_cast<float>(infos.imgsize.y));
    infos.sprite->SetOrigin({ 0.f, 0.f, 0.f }); // Bug fix until new version

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();
    gfx.Attach(infos.sprite, Def::CHARACTERS_LAYER);

    e->AddComponent<Ndk::NodeComponent>().SetPosition(infos.defG.x, infos.defG.y);

    e->AddComponent<LifeComponent>(infos.maxhp);
    e->AddComponent<FightComponent>();

    e->AddComponent<PositionComponent>(infos.defL);
    e->AddComponent<MapPositionComponent>(infos.mapPos);

    e->AddComponent<DefaultGraphicsPosComponent>(infos.defG);

    e->AddComponent<MoveComponent>();
    e->AddComponent<PathComponent>();
    e->AddComponent<InventoryComponent>();

    e->AddComponent<AnimationComponent>(infos.imgsize, infos.maxframe, infos.animState);
    e->AddComponent<OrientationComponent>(infos.o);

    if (infos.rdMov.randomMovement)
        e->AddComponent<RandomMovementComponent>(infos.rdMov.movInterval, infos.rdMov.nbTiles);

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

Ndk::EntityHandle make_item(const Ndk::WorldHandle& w, const Nz::String& codename, const Nz::String& name, 
                            const Nz::String& desc, unsigned level, Nz::TextureRef icon)
{
    Ndk::EntityHandle e = w->CreateEntity();

    e->AddComponent<Items::ItemComponent>(codename);
    e->AddComponent<NameComponent>().name = name;
    e->AddComponent<DescriptionComponent>().description = desc;
    e->AddComponent<LevelComponent>(level);
    e->AddComponent<IconComponent>().icon = icon;;

    return e;
}

extern Ndk::EntityHandle make_graphicalItem(const Ndk::WorldHandle& w, const Ndk::EntityHandle& logicItem, const Nz::Vector2f& size, const Nz::Vector2f& defGfxPos, int renderOrder)
{
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

    return e;
}
