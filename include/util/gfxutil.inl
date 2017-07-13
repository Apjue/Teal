// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class T>
T* getRenderableFromGraphicsComponent(Ndk::GraphicsComponent& gfx)
{
    static_assert(std::is_base_of<Nz::InstancedRenderable, T>::value, "T is not a renderable !");

    std::vector<Nz::InstancedRenderableRef> attached;
    gfx.GetAttachedRenderables(&attached);

    for (auto&& renderable : attached)
    {
        if (typeid(*(renderable.Get())) != typeid(T))
            continue;

        return static_cast<T*>(renderable.Get());
    }

    return nullptr;
}

void refreshGraphicsPos(const Ndk::EntityHandle& entity)
{
    refreshGraphicsPos(entity, entity);
}

