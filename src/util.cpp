// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util.hpp"

Nz::Sprite* getSpriteFromComponent(Ndk::GraphicsComponent& gfx)
{
    std::vector<Nz::InstancedRenderableRef> attached;
    gfx.GetAttachedRenderables(&attached);

    Nz::Sprite* sprite {};

    for (auto&& renderable : attached)
    {
        Nz::Sprite* casted = dynamic_cast<Nz::Sprite*>(renderable.Get());
        if (casted)
        {
            sprite = casted;
            break;
        }
    }

    return sprite;
}

namespace
{

Nz::ImageRef scheme {};

}

AbsTile getTileFromGlobalCoords(const Nz::Vector2ui& coords)
{
    NazaraAssert(scheme.IsValid(), "Scheme Ref isn't valid !");
    NazaraAssert(scheme->IsValid(), "Scheme Image isn't valid !");

    unsigned const x { coords.x }, y { coords.y };

    unsigned const rectX { (x / Def::TILEXSIZE) * 2 }; // Rectangle where we clicked
    unsigned const rectY { (y / Def::TILEYSIZE) * 2 }; // We need losange

    int const iRectX { static_cast<int>(rectX) };
    int const iRectY { static_cast<int>(rectY) };

    unsigned const rectClickX { x % Def::TILEXSIZE }; //We need the click to see
    unsigned const rectClickY { y % Def::TILEYSIZE }; //where in the rectangle we clicked

    Nz::Color color = scheme->GetPixelColor(rectClickX, rectClickY);

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

void setScheme(Nz::ImageRef newScheme)
{
    scheme = newScheme;
}

// QJsonDocument jsonFromFile(const QString& filename)
// {
//     QFile jsonFile { filename };
//     jsonFile.open(QFile::ReadOnly);
// 
//     return QJsonDocument().fromJson(jsonFile.readAll());
// }

// void jsonToFile(const QJsonDocument& document, const QString& filename)
// {
//     QFile jsonFile { filename };
//     jsonFile.open(QFile::WriteOnly);
//     jsonFile.write(document.toJson());
// }
