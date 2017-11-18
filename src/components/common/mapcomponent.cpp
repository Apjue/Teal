// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <Nazara/Math/Vector2.hpp>
#include "global.hpp"
#include "def/uidef.hpp"
#include "def/layerdef.hpp"
#include "util/assert.hpp"
#include "util/util.hpp"
#include "util/mapposutil.hpp"
#include "components/common/mapcomponent.hpp"

MapInstance::MapInstance(TilesetCore* tcore, TilesetCore* ftcore, const Ndk::EntityHandle& e)
    : m_entity(e), m_tilesetCore(tcore), m_fightTilesetCore(ftcore)
{
    NazaraAssert(m_entity->GetWorld(), "World is null");
    m_world = m_entity->GetWorld()->CreateHandle();

    m_tileset = Nz::Material::New("Translucent2D");

    m_tileset->EnableFaceCulling(true);
    m_tileset->SetFaceFilling(Nz::FaceFilling_Fill);

    auto matSampler = m_tileset->GetDiffuseSampler();
    matSampler.SetFilterMode(Nz::SamplerFilter_Nearest);
    m_tileset->SetDiffuseSampler(matSampler);


    m_fightTileset = Nz::Material::New("Translucent2D");

    m_fightTileset->EnableFaceCulling(true);
    m_fightTileset->SetFaceFilling(Nz::FaceFilling_Fill);

    m_tilemap = Nz::TileMap::New(Nz::Vector2ui { Def::MapX + 1, Def::MapY + 2 }, Nz::Vector2f { float(Def::TileSizeX), float(Def::TileSizeY) });
    m_tilemap->EnableIsometricMode(true);

    auto fightMatSampler = m_fightTileset->GetDiffuseSampler();
    fightMatSampler.SetFilterMode(Nz::SamplerFilter_Nearest);
    m_fightTileset->SetDiffuseSampler(fightMatSampler);


    if (!m_entity->HasComponent<Ndk::NodeComponent>())
        m_entity->AddComponent<Ndk::NodeComponent>();

    if (!m_entity->HasComponent<Ndk::GraphicsComponent>())
        m_entity->AddComponent<Ndk::GraphicsComponent>();

    auto& graphicsComponent = m_entity->GetComponent<Ndk::GraphicsComponent>();
    graphicsComponent.Attach(m_tilemap, Def::MapLayer);


    m_borderEntity = m_world->CreateEntity();

    auto& borderNode = m_borderEntity->AddComponent<Ndk::NodeComponent>();
    borderNode.SetParent(m_entity);
    borderNode.Move(-float(Def::TileSizeX / 2), -float(Def::TileSizeY / 2));

    auto& borderGfx = m_borderEntity->AddComponent<Ndk::GraphicsComponent>();

    for (unsigned i {}; i < m_leftBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_leftBorder[i];
        sprite = Nz::Sprite::New();

        sprite->SetSize(float(Def::TileSizeX), float(Def::TileSizeY));
        borderGfx.Attach(sprite, Nz::Matrix4f::Translate(Nz::Vector3f { 0.f, float(i * Def::TileSizeY), 0.f }), Def::MapLayer);
    }

    for (unsigned i {}; i < m_rightBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_rightBorder[i];
        sprite = Nz::Sprite::New();

        sprite->SetSize(float(Def::TileSizeX), float(Def::TileSizeY));
        borderGfx.Attach(sprite, Nz::Matrix4f::Translate(Nz::Vector3f { float(Def::MapSizeX), float(i * Def::TileSizeY), 0.f }), Def::MapLayer);
    }

    for (unsigned i {}; i < m_upBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_upBorder[i];
        sprite = Nz::Sprite::New();

        sprite->SetSize(float(Def::TileSizeX), float(Def::TileSizeY));
        borderGfx.Attach(sprite, Nz::Matrix4f::Translate(Nz::Vector3f { float(i * Def::TileSizeX + Def::TileSizeX), 0.f, 0.f }), Def::MapLayer);
    }

    for (unsigned i {}; i < m_downBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_downBorder[i];
        sprite = Nz::Sprite::New();

        sprite->SetSize(float(Def::TileSizeX), float(Def::TileSizeY / 2));
        borderGfx.Attach(sprite, Nz::Matrix4f::Translate(Nz::Vector3f { float(i * Def::TileSizeX + Def::TileSizeX / 2), float(Def::MapSizeY), 0.f }), Def::MapLayer);
    }
}

void MapInstance::update()
{
    TealAssert(m_tilesetCore, "TilesetCore nullptr !");
    TealAssert(m_fightTilesetCore, "Fight TilesetCore nullptr !");

    TilesetCore* tcore = (m_fightMode ? m_fightTilesetCore : m_tilesetCore);
    Nz::MaterialRef material = (m_fightMode ? m_fightTileset : m_tileset);

    m_tilemap->SetMaterial(0, material);

    for (unsigned i {}; i < Def::TileArraySize; ++i)
    {
        auto& tile = getCurrentMap()->tile(i);
        auto  pos = IndexToXY(i);
        Nz::Vector2ui tilePos { pos.first, pos.second };

        if (tile.isVisible())
        {
            Nz::Rectui tileRect { tcore->get(m_fightMode ? tile.fightTextureId : tile.textureId) * Def::TileSizeX, 0u, Def::TileSizeX, Def::TileSizeY };
            m_tilemap->EnableTile(tilePos, tileRect);
        }

        else
            m_tilemap->DisableTile(tilePos);
    }

    updateBorders();
}

void MapInstance::updateBorders()
{
    TealAssert(m_tilesetCore, "TilesetCore nullptr !");
    TealAssert(m_fightTilesetCore, "Fight TilesetCore nullptr !");

    TilesetCore* tcore = (m_fightMode ? m_fightTilesetCore : m_tilesetCore);
    Nz::MaterialRef material = (m_fightMode ? m_fightTileset : m_tileset);

    for (unsigned i {}; i < m_leftBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_leftBorder[i];
        Nz::Rectui tileRect { tcore->get(getCurrentMap()->tile(0, (i == m_leftBorder.size() - 1 ? i - 1 : i ) * 2).textureId) * Def::TileSizeX, 0u, Def::TileSizeX, Def::TileSizeY };

        sprite->SetMaterial(m_tileset, false);
        sprite->SetTextureRect(tileRect);
    }

    for (unsigned i {}; i < m_rightBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_rightBorder[i];
        Nz::Rectui tileRect { tcore->get(getCurrentMap()->tile(Def::MapX, (i == m_rightBorder.size() - 1 ? i - 1 : i) * 2).textureId) * Def::TileSizeX, 0u, Def::TileSizeX, Def::TileSizeY };

        sprite->SetMaterial(m_tileset, false);
        sprite->SetTextureRect(tileRect);
    }

    for (unsigned i {}; i < m_upBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_upBorder[i];
        Nz::Rectui tileRect { tcore->get(getCurrentMap()->tile(i + 1, 0).textureId) * Def::TileSizeX, 0u, Def::TileSizeX, Def::TileSizeY };

        sprite->SetMaterial(m_tileset, false);
        sprite->SetTextureRect(tileRect);
    }

    for (unsigned i {}; i < m_downBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_downBorder[i];
        Nz::Rectui tileRect { tcore->get(getCurrentMap()->tile((i == m_downBorder.size() - 1 ? i - 1 : i), Def::MapY + 1).textureId) * Def::TileSizeX, 0u, Def::TileSizeX, Def::TileSizeY / 2 };

        sprite->SetMaterial(m_tileset, false);
        sprite->SetTextureRect(tileRect);
    }
}

bool MapInstance::adjacentPassable(unsigned sX, unsigned sY, unsigned eX, unsigned eY)
{
    if (!isPositionValid({ eX, eY }))
        return false;

    // Step 1.
    {
        const std::array<int, 8u>* mapDistanceX = isLineEven(sY) ? &Def::MapDistanceEvenX : &Def::MapDistanceUnevenX;
        const std::array<int, 8u>* mapDistanceY = isLineEven(sY) ? &Def::MapDistanceEvenY : &Def::MapDistanceUnevenY;
        bool found { false };

        for (unsigned i {}; i < Def::MapDistanceCost.size(); ++i)
        {
            if (sX + (*mapDistanceX)[i] == eX && sY + (*mapDistanceY)[i] == eY)
            {
                if (IndexToXY(XYToIndex(eX, eY)).second != sY && (*mapDistanceY)[i] == 0)
                    continue;

                else
                {
                    found = true;
                    break;
                }
            }
        }

        if (!found)
            return false;
    }

    // Step 2.
    {
        unsigned const tileindex = XYToIndex(eX, eY);
        auto& tile = getCurrentMap()->tile(tileindex);

        return tile.isWalkable();
    }
}

float MapInstance::LeastCostEstimate(void* nodeStart, void* nodeEnd)
{
    unsigned sX {}, sY {};
    NodeToXY(nodeStart, sX, sY);

    unsigned eX {}, eY {};
    NodeToXY(nodeEnd, eX, eY);


    unsigned rX { distance(sX, eX) },
             rY { distance(sY, eY) };

    unsigned const estimated { rX + rY };
    return float(estimated);
}

void MapInstance::AdjacentCost(void* node, std::vector<micropather::StateCost>* neighbors)
{
    TealAssert(neighbors, "Micropather neighbors null !");

    unsigned x {}, y {};
    NodeToXY(node, x, y);

    bool even = isLineEven(y);

    for (std::size_t i {}; i < Def::MapDistanceCost.size(); ++i)
    {
        int newX = x + (even ? Def::MapDistanceEvenX[i] : Def::MapDistanceUnevenX[i]);
        int newY = y + (even ? Def::MapDistanceEvenY[i] : Def::MapDistanceUnevenY[i]);

        if (adjacentPassable(x, y, newX, newY))
        {
            micropather::StateCost nodeCost = { XYToNode(newX, newY), Def::MapDistanceCost[i] };
            neighbors->push_back(nodeCost);
        }
    }
}
