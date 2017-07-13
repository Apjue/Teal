// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gfxutil.hpp"

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

void cloneRenderables(const Ndk::EntityHandle& e)
{
    cloneRenderables(e->GetComponent<Ndk::GraphicsComponent>(), e->GetComponent<RenderablesStorageComponent>());
}

void cloneRenderables(Ndk::GraphicsComponent& gfx, RenderablesStorageComponent& renderables)
{
    for (auto& sprite : renderables.sprites)
    {
        Nz::SpriteRef newSprite = Nz::Sprite::New(*(static_cast<Nz::Sprite*>(sprite.Get())));

        gfx.Detach(sprite);
        gfx.Attach(newSprite);

        sprite.Swap(newSprite);
        break;
    }

    for (auto& model : renderables.models)
    {
        Nz::ModelRef newModel = Nz::Model::New(*(static_cast<Nz::Model*>(model.Get())));

        gfx.Detach(model);
        gfx.Attach(newModel);

        model.Swap(newModel);

        break;
    }

    for (auto& textSprite : renderables.textSprites)
    {
        Nz::TextSpriteRef newTextSprite = Nz::TextSprite::New(*(static_cast<Nz::TextSprite*>(textSprite.Get())));

        gfx.Detach(textSprite);
        gfx.Attach(newTextSprite);

        textSprite.Swap(newTextSprite);

        break;
    }
}


void refreshGraphicsPos(const Ndk::EntityHandle& entity)
{
    refreshGraphicsPos(entity, entity);
}

