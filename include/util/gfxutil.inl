// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gfxutil.hpp"

void cloneRenderables(const Ndk::EntityHandle& e)
{
    cloneRenderables(e->GetComponent<Ndk::GraphicsComponent>(), e->GetComponent<RenderablesStorageComponent>());
}

void refreshGraphicsPos(const Ndk::EntityHandle& entity)
{
    refreshGraphicsPos(entity, entity);
}

