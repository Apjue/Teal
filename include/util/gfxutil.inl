// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gfxutil.hpp"

void cloneRenderables(const Ndk::EntityHandle& e, unsigned renderOrder)
{
    cloneRenderables(e->GetComponent<Ndk::GraphicsComponent>(), e->GetComponent<RenderablesStorageComponent>(), renderOrder);
}

