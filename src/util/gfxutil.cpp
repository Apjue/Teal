// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Nazara/Graphics/Sprite.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <Nazara/Core/Color.hpp>
#include <vector>
#include "def/gamedef.hpp"
#include "components/characters/positioncomponent.hpp"
#include "components/shared/graphicsoffsetcomponent.hpp"
#include "components/util/renderablesstoragecomponent.hpp"
#include "util/assert.hpp"
#include "util/mapposutil.hpp"
#include "util/entityutil.hpp"
#include "util/util.hpp"
#include "util/gfxutil.hpp"

namespace
{

Nz::ImageRef m_scheme {};

}

void cloneRenderables(Ndk::GraphicsComponent& gfx, RenderablesStorageComponent& renderables, unsigned renderOrder)
{
    for (auto& sprite : renderables.sprites)
    {
        Nz::SpriteRef newSprite = Nz::Sprite::New(*sprite.Get());

        gfx.Detach(sprite);
        gfx.Attach(newSprite, renderOrder);

        sprite = newSprite;
    }

    for (auto& model : renderables.models)
    {
        Nz::ModelRef newModel = Nz::Model::New(*model.Get());

        gfx.Detach(model);
        gfx.Attach(newModel, renderOrder);

        model = newModel;
    }

    for (auto& textSprite : renderables.textSprites)
    {
        Nz::TextSpriteRef newTextSprite = Nz::TextSprite::New(*textSprite.Get());

        gfx.Detach(textSprite);
        gfx.Attach(newTextSprite, renderOrder);

        textSprite = newTextSprite;
    }
}

AbsTile getTileFromGlobalCoords(const Nz::Vector2ui& coords)
{
    TealAssert(m_scheme.IsValid(), "Scheme Ref isn't valid, setScheme() must be used !");
    TealAssert(m_scheme->IsValid(), "Scheme Image isn't valid !");

    unsigned const x { coords.x }, y { coords.y };

    unsigned const rectX { (x / Def::TileSizeX)      }; // Rectangle where we clicked
    unsigned const rectY { (y / Def::TileSizeY) * 2u }; // We need losange

    unsigned const rectClickX { x % Def::TileSizeX }; // We need the click to see
    unsigned const rectClickY { y % Def::TileSizeY }; // where in the rectangle we clicked

    Nz::Color color = m_scheme->GetPixelColor(rectClickX, rectClickY);

    int losangeX { int(rectX) };
    int losangeY { int(rectY) };

    bool even = isLineEven(rectY);
    Orientation orientation = Orientation::Down;

    if (color == Nz::Color::Blue)
        orientation = Orientation::UpLeft;

    else if (color == Nz::Color::Red)
        orientation = Orientation::UpRight;

    else if (color == Nz::Color::Yellow)
        orientation = Orientation::DownLeft;

    else if (color == Nz::Color::Green)
        orientation = Orientation::DownRight;

    else if (color == Nz::Color::White)
        orientation = Orientation::Up;

    else
        NazaraError(Nz::String { "Error in scheme ! [with color = " }.Append(color.ToString()).Append(" & pixel pos = ")
                    .Append(Nz::Vector2ui { rectClickX, rectClickY }.ToString()).Append("]"));

    if (orientation != Orientation::Up)
    {
        losangeX += even ? Def::MapDistanceEvenX[toUnderlyingType(orientation)] : Def::MapDistanceEvenX[toUnderlyingType(orientation)];
        losangeY += even ? Def::MapDistanceEvenY[toUnderlyingType(orientation)] : Def::MapDistanceEvenY[toUnderlyingType(orientation)];
    }


    // If the tile is negative:
    losangeX = (losangeX < 0) ? 0 : losangeX;
    losangeY = (losangeY < 0) ? 0 : losangeY;

    unsigned fLosangeX { unsigned(losangeX) };
    unsigned fLosangeY { unsigned(losangeY) };

    // If tile is out the map:
    fLosangeX = (fLosangeX > Def::ArrayMapX) ? Def::ArrayMapX : fLosangeX;
    fLosangeY = (fLosangeY > Def::ArrayMapY) ? Def::ArrayMapY : fLosangeY;

    return { fLosangeX, fLosangeY };
}

extern void initializeSchemeUtility(const Nz::ImageRef& newScheme)
{
    m_scheme = newScheme;
}

void refreshGraphicsPos(const Ndk::EntityHandle& entity)
{
    TealAssert(isMapEntity(entity), "Graphical Entity isn't a graphical entity !");

    auto& pos = entity->GetComponent<PositionComponent>();
    auto& node = entity->GetComponent<Ndk::NodeComponent>();
    Nz::Vector2f defPos = getGraphicsOffset(entity);

    unsigned const gX = pos.xy.x * Def::TileSizeX + (isLineEven(pos.xy.y) ? 0u : Def::TileSizeX / 2); // convert logic pos to graphics pos
    unsigned const gY = pos.xy.y * Def::TileSizeY / 2;

    DiffTile gInXY {};

    if (pos.direction)
    {
        gInXY = DirToGXY(pos.direction);
        gInXY.x *= (pos.advancement * Def::MaxGraphicsXPosInTile);
        gInXY.y *= (pos.advancement * Def::MaxGraphicsYPosInTile);
    }

    // We will move using this (so it's graphics pos)
    Nz::Vector3f finalPos { float(gX) + float(gInXY.x) + defPos.x, float(gY) + float(gInXY.y) + defPos.y, 0.f };
    finalPos.z = finalPos.y / 10.f;

    if (finalPos.x != node.GetPosition().x || finalPos.y != node.GetPosition().y) // if the entity is already at that position, no need to move it
        node.SetPosition(finalPos);
}
