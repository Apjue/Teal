// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef LOGICENTITYIDCOMPONENT_HPP
#define LOGICENTITYIDCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <NDK/Entity.hpp>

///
/// \struct LogicEntityIdComponent
///
/// \brief Used by any function or class requiring to have a sprite using data from a logic entity
///
/// \example Items on the ground, items in inventory GUI (todo)
///

struct LogicEntityIdComponent : public Ndk::Component<LogicEntityIdComponent>
{
    Ndk::EntityHandle logicEntity;

    static Ndk::ComponentIndex componentIndex;
};

#endif // LOGICENTITYIDCOMPONENT_HPP
