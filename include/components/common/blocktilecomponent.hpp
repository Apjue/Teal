// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef BLOCKTILECOMPONENT_HPP
#define BLOCKTILECOMPONENT_HPP

#include <NDK/Component.hpp>

struct BlockTileComponent : public Ndk::Component<BlockTileComponent>
{
    bool blockTile { false };

    // Todo: Number of tiles occupied ?

    static Ndk::ComponentIndex componentIndex;
};

#endif // BLOCKTILECOMPONENT_HPP
