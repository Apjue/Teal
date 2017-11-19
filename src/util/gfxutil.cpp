// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Nazara/Graphics/Sprite.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <Nazara/Core/Color.hpp>
#include <vector>
#include "def/gamedef.hpp"
#include "components/common/positioncomponent.hpp"
#include "components/common/defaultgraphicsposcomponent.hpp"
#include "util/assert.hpp"
#include "util/mapposutil.hpp"
#include "util/entityutil.hpp"
#include "util/util.hpp"
#include "util/gfxutil.hpp"

namespace
{

Nz::ImageRef m_scheme {};

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
    Orientation o = Orientation::Down;


    if (color == Nz::Color::Blue)
        o = Orientation::UpLeft;

    if (color == Nz::Color::Red)
        o = Orientation::UpRight;

    if (color == Nz::Color::Yellow)
        o = Orientation::DownLeft;

    if (color == Nz::Color::Green)
        o = Orientation::DownRight;

    if (color == Nz::Color::White)
        o = Orientation::Up;


    if (o == Orientation::Down)
        NazaraError("Error in scheme !");

    if (o != Orientation::Up)
    {
        losangeX += even ? Def::MapDistanceEvenX[toUnderlyingType(o)] : Def::MapDistanceEvenX[toUnderlyingType(o)];
        losangeY += even ? Def::MapDistanceEvenY[toUnderlyingType(o)] : Def::MapDistanceEvenY[toUnderlyingType(o)];
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

void initSchemeUtility(Nz::ImageRef newScheme)
{
    m_scheme = newScheme;
}

void refreshGraphicsPos(const Ndk::EntityHandle& logicEntity, const Ndk::EntityHandle& graphicalEntity)
{
    TealAssert(isMapEntity(graphicalEntity), "Graphical Entity isn't a graphical entity !");

    auto& pos = logicEntity->GetComponent<PositionComponent>();
    auto& gfxcomp = graphicalEntity->GetComponent<Ndk::GraphicsComponent>();
    auto& node = graphicalEntity->GetComponent<Ndk::NodeComponent>();
    Nz::Vector2f defPos = getDefGfxPos(graphicalEntity);

    unsigned const gX = pos.xy.x * Def::TileSizeX + (isLineEven(pos.xy.y) ? 0u : Def::TileSizeX / 2); // convert logic pos to graphics pos
    unsigned const gY = pos.xy.y * Def::TileSizeY / 2;

    DiffTile gInXY {};

    if (pos.direction)
    {
        gInXY = DirToGXY(pos.direction);
        gInXY.x *= (pos.advancement * Def::MaxGXPosInTile);
        gInXY.y *= (pos.advancement * Def::MaxGYPosInTile);
    }

    float const finalX = float(gX) + float(gInXY.x) + defPos.x; // We will move using this
    float const finalY = float(gY) + float(gInXY.y) + defPos.y; // (so it's graphics pos)

    if (finalX != node.GetPosition().x || // if the entity is already at that position
        finalY != node.GetPosition().y) // no need to move it
    {
        float const moveX = finalX - node.GetPosition().x;
        float const moveY = finalY - node.GetPosition().y;

        node.Move(moveX, moveY);
    }
}
