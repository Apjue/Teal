// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <algorithm>
#include "components/common/clonecomponent.hpp"
#include "components/common/graphicalentitiescomponent.hpp"
#include "components/common/logicentityidcomponent.hpp"
#include "def/layerdef.hpp"
#include "util/gfxutil.hpp"
#include "util/cloneutil.hpp"

Ndk::EntityHandle cloneCharacter(const Ndk::EntityList& characters, const Nz::String& codename)
{
    Ndk::EntityHandle entity;
    auto it = std::find_if(characters.begin(), characters.end(),
                           [&codename] (const Ndk::EntityHandle& e) { return e->HasComponent<CloneComponent>() && (e->GetComponent<CloneComponent>().codename == codename); });

    if (it != characters.end())
    {
        entity = (*it)->Clone();
        cloneRenderables(entity, Def::CharactersLayer);
    }

    return entity;
}

Ndk::EntityHandle cloneItem(const Ndk::EntityList& items, const Nz::String& codename)
{
    Ndk::EntityHandle entity;
    auto it = std::find_if(items.begin(), items.end(),
                           [&codename] (const Ndk::EntityHandle& e) { return e->HasComponent<CloneComponent>() && e->GetComponent<CloneComponent>().codename == codename; });

    if (it != items.end())
    {
        entity = (*it)->Clone();

        if (!entity->HasComponent<GraphicalEntitiesComponent>())
            return entity;

        auto& gfxEntities = entity->GetComponent<GraphicalEntitiesComponent>();
        Ndk::EntityList newEntities;

        for (auto& gfxEntity : gfxEntities.entities)
        {
            Ndk::EntityHandle newEntity = gfxEntity->Clone();
            auto& logicComp = newEntity->GetComponent<LogicEntityIdComponent>();

            cloneRenderables(newEntity, logicComp.getRenderOrder());

            logicComp.logicEntity = entity;
            newEntities.Insert(newEntity);
        }

        gfxEntities.entities = newEntities;
    }

    return entity;
}

