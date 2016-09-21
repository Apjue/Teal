// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

unsigned dToU(double d)
{
    return static_cast<unsigned>(d);
}

template<class T>
T distance(const T& x, const T& y)
{
    return (x > y) ? x - y : y - x;
}

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
        {
            rdable = casted;
            break;
        }
    }

    return rdable;
}

DiffTile AbsPosToDiff(const AbsTile& from, const AbsTile& to)
{
    int diffX { static_cast<int>(to.x) - static_cast<int>(from.x) },
        diffY { static_cast<int>(to.y) - static_cast<int>(from.y) };

    return { diffX, diffY };
}
