// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef RENDERABLESSTORAGECOMPONENT_HPP
#define RENDERABLESSTORAGECOMPONENT_HPP

#include <NDK/Component.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Graphics/Model.hpp>
#include <Nazara/Graphics/TextSprite.hpp>
#include <vector>

struct RenderablesStorageComponent : public Ndk::Component<RenderablesStorageComponent>
{
    std::vector<Nz::SpriteRef> sprites; // Before adding another InstancedRenderable, see cloneRenderables in gfxutil
    std::vector<Nz::ModelRef> models;
    std::vector<Nz::TextSpriteRef> textSprites;

    static Ndk::ComponentIndex componentIndex;
};

#endif // RENDERABLESSTORAGECOMPONENT_HPP
