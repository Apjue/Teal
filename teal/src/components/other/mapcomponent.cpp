// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
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
#include "util/nzstlcompatibility.hpp"
#include "components/shared/positioncomponent.hpp"
#include "components/shared/blocktilecomponent.hpp"
#include "components/other/mapobjectcomponent.hpp"
#include "components/other/mapcomponent.hpp"

MapInstance::MapInstance(const TilesetCore& fightTilesetCore, const Ndk::EntityHandle& e)
    : m_entity(e), m_fightTilesetCore(fightTilesetCore)
{
    NazaraAssert(m_entity->GetWorld(), "World is null");
    m_world = m_entity->GetWorld()->CreateHandle();

    // Initialize tileset
    {
        m_tileset = Nz::Material::New("Translucent2D");

        m_tileset->EnableFaceCulling(true);
        m_tileset->SetFaceFilling(Nz::FaceFilling_Fill);

        auto matSampler = m_tileset->GetDiffuseSampler();
        matSampler.SetFilterMode(Nz::SamplerFilter_Nearest);
        m_tileset->SetDiffuseSampler(matSampler);
    }

    // Initialize fight tileset
    {
        m_fightTileset = Nz::Material::New("Translucent2D");

        m_fightTileset->EnableFaceCulling(true);
        m_fightTileset->SetFaceFilling(Nz::FaceFilling_Fill);

        auto fightMatSampler = m_fightTileset->GetDiffuseSampler();
        fightMatSampler.SetFilterMode(Nz::SamplerFilter_Nearest);
        m_fightTileset->SetDiffuseSampler(fightMatSampler);
    }

    // Add needed components
    {
        if (!m_entity->HasComponent<Ndk::NodeComponent>())
            m_entity->AddComponent<Ndk::NodeComponent>();

        if (!m_entity->HasComponent<Ndk::GraphicsComponent>())
            m_entity->AddComponent<Ndk::GraphicsComponent>();
    }

    m_tilemap = Nz::TileMap::New(Nz::Vector2ui { Def::ArrayMapX, Def::ArrayMapY + 1 }, Nz::Vector2f { float(Def::TileSizeX), float(Def::TileSizeY) });
    m_tilemap->EnableIsometricMode(true);                                  // ^ + 1 because of stagger axis things...

    auto& graphicsComponent = m_entity->GetComponent<Ndk::GraphicsComponent>();
    graphicsComponent.Attach(m_tilemap, Def::MapLayer);

    // Initialize borders
    {
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
            borderGfx.Attach(sprite, Nz::Matrix4f::Translate(Nz::Vector3f { 0.f, float(i * Def::TileSizeY), 0.f }), Def::MapBordersLayer);
        }

        for (unsigned i {}; i < m_rightBorder.size(); ++i)
        {
            Nz::SpriteRef& sprite = m_rightBorder[i];
            sprite = Nz::Sprite::New();

            sprite->SetSize(float(Def::TileSizeX), float(Def::TileSizeY));
            borderGfx.Attach(sprite, Nz::Matrix4f::Translate(Nz::Vector3f { float(Def::MapSizeX), float(i * Def::TileSizeY), 0.f }), Def::MapBordersLayer);
        }

        for (unsigned i {}; i < m_upBorder.size(); ++i)
        {
            Nz::SpriteRef& sprite = m_upBorder[i];
            sprite = Nz::Sprite::New();

            sprite->SetSize(float(Def::TileSizeX), float(Def::TileSizeY));
            borderGfx.Attach(sprite, Nz::Matrix4f::Translate(Nz::Vector3f { float(i * Def::TileSizeX + Def::TileSizeX), 0.f, 0.f }), Def::MapBordersLayer);
        }

        for (unsigned i {}; i < m_downBorder.size(); ++i)
        {
            Nz::SpriteRef& sprite = m_downBorder[i];
            sprite = Nz::Sprite::New();

            sprite->SetSize(float(Def::TileSizeX), float(Def::TileSizeY / 2));
            borderGfx.Attach(sprite, Nz::Matrix4f::Translate(Nz::Vector3f { float(i * Def::TileSizeX + Def::TileSizeX / 2), float(Def::MapSizeY), 0.f }), Def::MapBordersLayer);
        }
    }
}

void MapInstance::update()
{
    //m_fightMode = true; /// TEST
    m_tilemap->SetMaterial(0, (m_fightMode ? m_fightTileset : m_tileset));

    std::vector<unsigned> blockedTiles; // Map objects block these tiles

    if (m_fightMode)
        for (auto& e : getCurrentMap()->getGraphicalEntities()) // current map index only changes when entering into a house or something similar
            if (e->HasComponent<MapObjectComponent>() && e->HasComponent<BlockTileComponent>()) // But NOT when switching to fight mode
            {
                e->Enable(false); /// BUG

                auto& tiles = e->GetComponent<BlockTileComponent>().occupied;
                auto& xy = e->GetComponent<PositionComponent>().xy;

                if (tiles.empty())
                    blockedTiles.push_back(XYToIndex(xy.x, xy.y));

                else
                    for (auto& tile : tiles)
                    {
                        bool ok = false;
                        AbsTile resultTile = applyDiffTile(xy, tile, ok);

                        if (ok)
                            blockedTiles.push_back(XYToIndex(resultTile.x, resultTile.y));
                    }
            }

    for (unsigned i {}; i < Def::TileArraySize; ++i)
    {
        const TileData& tile = getCurrentMap()->getTile(i);
        AbsTile tilePos { toVector2(IndexToXY(i)) };

        if (tile.isVisible())
        {
            Nz::Rectui tileRect { 0u, 0u, Def::TileSizeX, Def::TileSizeY };

            if (m_fightMode)
            {
                if (std::find(blockedTiles.begin(), blockedTiles.end(), i) != blockedTiles.end())
                    tileRect.x = m_fightTilesetCore.get("empty") * Def::TileSizeX;

                else
                    switch (tile.flag)
                    {
                        case TileFlag::NoFlag:
                            tileRect.x = m_fightTilesetCore.get("walk") * Def::TileSizeX;
                            break;

                        case TileFlag::Invisible:
                            throw std::runtime_error { "Tile is visible but has Invisible flag!" };
                            break;

                        case TileFlag::ViewObstacle:
                            tileRect.x = m_fightTilesetCore.get("empty") * Def::TileSizeX;
                            break;

                        case TileFlag::BlockObstacle:
                            tileRect.x = m_fightTilesetCore.get("empty") * Def::TileSizeX;
                            break;

                        case TileFlag::RedSpawn:
                            tileRect.x = m_fightTilesetCore.get("red") * Def::TileSizeX;
                            break;

                        case TileFlag::BlueSpawn:
                            tileRect.x = m_fightTilesetCore.get("blue") * Def::TileSizeX;
                            break;
                    }
            }

            else
                tileRect.x = tile.textureId * Def::TileSizeX;

            m_tilemap->EnableTile(tilePos, tileRect);
        }

        else
            m_tilemap->DisableTile(tilePos);
    }

    updateBorders();
}

void MapInstance::updateBorders()
{
    if (m_fightMode)
    {
        m_borderEntity->Enable(false);
        return;
    }

    else
        m_borderEntity->Enable();

    for (unsigned i {}; i < m_leftBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_leftBorder[i];
        const TileData& tileData = getCurrentMap()->getTile(0, (i == m_leftBorder.size() - 1 ? i - 1 : i) * 2);

        if (!tileData.isVisible())
            sprite->SetMaterial(Nz::Material::New("Translucent2D"), false);

        else
        {
            Nz::Rectui tileRect { tileData.textureId * Def::TileSizeX, 0u, Def::TileSizeX, Def::TileSizeY };

            sprite->SetMaterial(m_tileset, false);
            sprite->SetTextureRect(tileRect);
        }
    }

    for (unsigned i {}; i < m_rightBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_rightBorder[i];
        const TileData& tileData = getCurrentMap()->getTile(Def::MapX, (i == m_rightBorder.size() - 1 ? i - 1 : i) * 2);

        if (!tileData.isVisible())
            sprite->SetMaterial(Nz::Material::New("Translucent2D"), false);

        else
        {
            Nz::Rectui tileRect { tileData.textureId * Def::TileSizeX, 0u, Def::TileSizeX, Def::TileSizeY };

            sprite->SetMaterial(m_tileset, false);
            sprite->SetTextureRect(tileRect);
        }
    }

    for (unsigned i {}; i < m_upBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_upBorder[i];
        const TileData& tileData = getCurrentMap()->getTile(i + 1, 0);

        if (!tileData.isVisible())
            sprite->SetMaterial(Nz::Material::New("Translucent2D"), false);

        else
        {
            Nz::Rectui tileRect { tileData.textureId * Def::TileSizeX, 0u, Def::TileSizeX, Def::TileSizeY };

            sprite->SetMaterial(m_tileset, false);
            sprite->SetTextureRect(tileRect);
        }
    }

    for (unsigned i {}; i < m_downBorder.size(); ++i)
    {
        Nz::SpriteRef& sprite = m_downBorder[i];
        const TileData& tileData = getCurrentMap()->getTile((i == m_downBorder.size() - 1 ? i - 1 : i), Def::MapY + 1);

        if (!tileData.isVisible())
            sprite->SetMaterial(Nz::Material::New("Translucent2D"), false);

        else
        {
            Nz::Rectui tileRect { tileData.textureId * Def::TileSizeX, 0u, Def::TileSizeX, Def::TileSizeY / 2 };

            sprite->SetMaterial(m_tileset, false);
            sprite->SetTextureRect(tileRect);
        }
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
        auto& tile = getCurrentMap()->getTile(tileindex);

        return tile.isWalkable() && tile.isVisible();
    }
}

float MapInstance::LeastCostEstimate(void* nodeStart, void* nodeEnd)
{
    unsigned sX {}, sY {};
    NodeToXY(nodeStart, sX, sY);

    unsigned eX {}, eY {};
    NodeToXY(nodeEnd, eX, eY);

    return float(distanceBetweenTiles({ sX, sY }, { eX, eY }));
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
