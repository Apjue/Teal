// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "factory.hpp"

Ndk::EntityHandle make_character(Ndk::WorldHandle& w, const CharacterInfos& c)
{
    Ndk::EntityHandle e = w->CreateEntity();

    auto& gfx = e->AddComponent<Ndk::GraphicsComponent>();
    gfx.Attach(c.sprite);

    e->AddComponent<Ndk::NodeComponent>();

    e->AddComponent<Components::Life>(c.maxhp);
    e->AddComponent<Components::Fight>();

    e->AddComponent<Components::CDirection>(c.o);
    e->AddComponent<Components::Position>(c.defL.x, c.defL.y);

    auto& dpos = e->AddComponent<Components::DefaultGraphicsPos>(c.defG.x, c.defG.y);
    e->GetComponent<Ndk::NodeComponent>().SetPosition(dpos.x, dpos.y);


    e->AddComponent<Components::MoveTo>();
    e->AddComponent<Components::Path>();
    //e->addComponent<Components::Inventory>();

    e->AddComponent<Components::Animation>(c.imgsize, c.maxframe, c.animState);

    w->Update();
    return e;
}
