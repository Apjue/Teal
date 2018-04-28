// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ACTORCOMPONENT_HPP
#define ACTORCOMPONENT_HPP

#include <NDK/Component.hpp>

struct ActorComponent : public Ndk::Component<ActorComponent>
{
    enum ActorType
    {
        Player,
        Monster,
        NPC
    } actorType;
};

#endif // ACTORCOMPONENT_HPP
