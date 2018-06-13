// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ICONCOMPONENT_HPP
#define ICONCOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Renderer/Texture.hpp>

struct IconComponent : public Ndk::Component<IconComponent>
{
    IconComponent() = default;

    Nz::TextureRef icon;

    static Ndk::ComponentIndex componentIndex;
};

#endif // ICONCOMPONENT_HPP
