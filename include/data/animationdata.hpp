// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMATIONDATA_HPP
#define ANIMATIONDATA_HPP

#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Renderer/Texture.hpp>

struct AnimationData // todo: put that in doublestore
{
    enum AnimationType
    {
        Walk, // In map, normal animation
        Fight,
        Emote
    };

    unsigned frame {}; // frame * size of the image = vertical coords of the image
    Nz::Vector2ui size {};
    Nz::TextureRef texture; // Texture to apply on the sprite
};

#endif // ANIMATIONDATA_HPP
