// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <Nazara/Math/Vector2.hpp>
#include "def/layerdef.hpp"
#include "util/assert.hpp"
#include "util/util.hpp"
#include "util/mapposutil.hpp"
#include "components/common/mapcomponent.hpp"

MapInstance::MapInstance(TilesetCore* tcore, TilesetCore* ftcore, const Ndk::EntityHandle& e)
    : m_entity(e), m_tilesetCore(tcore), m_fightTilesetCore(ftcore)
{
    m_tileset = Nz::Material::New("Translucent2D");

    m_tileset->EnableFaceCulling(true);
    m_tileset->SetFaceFilling(Nz::FaceFilling_Fill);

    auto matSampler = m_tileset->GetDiffuseSampler();
    matSampler.SetFilterMode(Nz::SamplerFilter_Nearest);
    m_tileset->SetDiffuseSampler(matSampler);


    m_fightTileset = Nz::Material::New("Translucent2D");

    m_fightTileset->EnableFaceCulling(true);
    m_fightTileset->SetFaceFilling(Nz::FaceFilling_Fill);

    m_tilemap = Nz::TileMap::New(Nz::Vector2ui { Def::MapX + 1, Def::MapY + 2 }, Nz::Vector2f { static_cast<float>(Def::TileSizeX), static_cast<float>(Def::TileSizeY) });
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
}

void MapInstance::update()
{
    TealAssert(m_tilesetCore, "TilesetCore nullptr !");
    TealAssert(m_fightTilesetCore, "Fight TilesetCore nullptr !");
    TealAssert(m_map, "Map is not valid !");

    TilesetCore* tcore = (m_fightMode ? m_fightTilesetCore : m_tilesetCore);

    Nz::MaterialRef material = (m_fightMode ? m_fightTileset : m_tileset);
    m_tilemap->SetMaterial(0, material);

    for (unsigned i {}; i < Def::TileArraySize; ++i)
    {
        auto& tile = m_map->tile(i);
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
}

bool MapInstance::adjacentPassable(unsigned sX, unsigned sY, unsigned eX, unsigned eY)
{
    TealAssert(m_map, "Map is not valid !");

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
        auto& tile = m_map->tile(tileindex);

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
    return static_cast<float>(estimated);
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
