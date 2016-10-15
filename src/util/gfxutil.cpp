// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/gfxutil.hpp"

namespace
{

Nz::ImageRef m_scheme {};

}

AbsTile getTileFromGlobalCoords(const Nz::Vector2ui& coords)
{
    NazaraAssert(m_scheme.IsValid(), "Scheme Ref isn't valid, setScheme() must be used !");
    NazaraAssert(m_scheme->IsValid(), "Scheme Image isn't valid !");

    unsigned const x { coords.x }, y { coords.y };

    unsigned const rectX { (x / Def::TILEXSIZE) * 2 }; // Rectangle where we clicked
    unsigned const rectY { (y / Def::TILEYSIZE) * 2 }; // We need losange

    int const iRectX { static_cast<int>(rectX) };
    int const iRectY { static_cast<int>(rectY) };

    unsigned const rectClickX { x % Def::TILEXSIZE }; //We need the click to see
    unsigned const rectClickY { y % Def::TILEYSIZE }; //where in the rectangle we clicked

    Nz::Color color = m_scheme->GetPixelColor(rectClickX, rectClickY);

    int losangeX {};
    int losangeY {};

    //Blue == up left
    if (color == Nz::Color::Blue)
    {
        losangeX = iRectX - 1;
        losangeY = iRectY - 1;
    }

    //Red == up right
    if (color == Nz::Color::Red)
    {
        losangeX = iRectX + 1;
        losangeY = iRectY - 1;
    }

    //Yellow == down left
    if (color == Nz::Color::Yellow)
    {
        losangeX = iRectX - 1;
        losangeY = iRectY + 1;
    }

    //Green == down right
    if (color == Nz::Color::Green)
    {
        losangeX = iRectX + 1;
        losangeY = iRectY + 1;
    }

    //White == The tile.
    if (color == Nz::Color::White)
    {
        losangeX = iRectX;
        losangeY = iRectY;
    }

    //If the tile is negative:
    losangeX = (losangeX < 0) ? 0 : losangeX;
    losangeY = (losangeY < 0) ? 0 : losangeY;

    unsigned fLosangeX { static_cast<unsigned>(losangeX) };
    unsigned fLosangeY { static_cast<unsigned>(losangeY) };

    //If tile is out the map:
    fLosangeX = (fLosangeX > Def::LMAPX) ? Def::LMAPX : fLosangeX;
    fLosangeY = (fLosangeY > Def::LMAPY) ? Def::LMAPY : fLosangeY;

    return { fLosangeX, fLosangeY };
}

void initSchemeUtility(Nz::ImageRef newScheme)
{
    m_scheme = newScheme;
}

void refreshGraphicsPos(const Ndk::EntityHandle& charac)
{
    auto& pos = charac->GetComponent<PositionComponent>();
    auto& gfxcomp = charac->GetComponent<Ndk::GraphicsComponent>();
    auto& gfxpos = charac->GetComponent<Ndk::NodeComponent>();
    auto& dpos = charac->GetComponent<DefaultGraphicsPosComponent>();

    Nz::Vector2f defPos { dpos.x, dpos.y };

    unsigned const gX = pos.x * Def::TILEGXSIZE; // convert logic pos to graphics pos
    unsigned const gY = pos.y * Def::TILEGYSIZE;
    int const gInX = pos.inX * Def::MAXGXPOSINTILE;
    int const gInY = pos.inY * Def::MAXGYPOSINTILE;

    float finalX = static_cast<float>(gX) + static_cast<float>(gInX); // We will move using this
    float finalY = static_cast<float>(gY) + static_cast<float>(gInY); // (so it's graphics pos)

    finalX += defPos.x;
    finalY += defPos.y;

    if (finalX != gfxpos.GetPosition().x  // if the entity is already at that position
     || finalY != gfxpos.GetPosition().y) // no need to move it
    {
        float const moveX = finalX - gfxpos.GetPosition().x;
        float const moveY = finalY - gfxpos.GetPosition().y;

        gfxpos.Move(moveX, moveY);
    }
}
