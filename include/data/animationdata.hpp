// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ANIMATIONDATA_HPP
#define ANIMATIONDATA_HPP

#include <Nazara/Core/String.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Renderer/Texture.hpp>

struct AnimationData
{
    enum AnimationType
    {
        Walk, // In map, normal animation
        Fight,
        Emote
    };

    static AnimationType stringToAnimationType(Nz::String string)
    {
        string = string.ToLower();

        if (string == "walk")
            return Walk;

        if (string == "fight")
            return Fight;

        if (string == "emote")
            return Emote;

        return {};
    }

    AnimationType type {};
    Nz::Vector2ui size {};
    Nz::TextureRef texture; // Texture to apply on the sprite
    Nz::Vector2f offset {}; // offset to apply for the character to be on pos [1;1]
    unsigned frame {}; // frame * y-size of the texture = vertical coords
};

#endif // ANIMATIONDATA_HPP
