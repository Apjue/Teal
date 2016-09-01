// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "factory.hpp"

Ndk::EntityHandle make_character(Ndk::WorldHandle& w, const CharacterInfos& infos)
{
    Ndk::EntityHandle e = w->CreateEntity();

    infos.sprite->SetSize(static_cast<float>(infos.imgsize.x), static_cast<float>(infos.imgsize.y));

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();
    gfx.Attach(infos.sprite/*, Def::CHARACTERS_LAYER*/);

    e->AddComponent<Ndk::NodeComponent>();

    e->AddComponent<Components::Life>(infos.maxhp);
    e->AddComponent<Components::Fight>();

    e->AddComponent<Components::CDirection>(infos.o);
    e->AddComponent<Components::Position>(infos.defL.x, infos.defL.y);

    /*auto& dpos = */e->AddComponent<Components::DefaultGraphicsPos>(infos.defG.x, infos.defG.y);
//     e->GetComponent<Ndk::NodeComponent>().SetPosition(dpos.x, dpos.y);


    e->AddComponent<Components::MoveTo>();
    e->AddComponent<Components::Path>();
    //e->addComponent<Components::Inventory>();

    e->AddComponent<Components::Animation>(infos.imgsize, infos.maxframe, infos.animState);

    return e;
}
