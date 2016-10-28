// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class T>
T* getRenderableFromGraphicsComponent(Ndk::GraphicsComponent& gfx)
{
    static_assert(std::is_base_of<Nz::InstancedRenderable, T>::value, "T is not a renderable !");

    std::vector<Nz::InstancedRenderableRef> attached;
    gfx.GetAttachedRenderables(&attached);

    T* rdable {};

    for (auto&& renderable : attached)
    {
        T* casted = dynamic_cast<T*>(renderable.Get());

        if (casted)
            return casted;
    }

    return rdable;
}
