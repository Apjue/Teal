// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/common/mapcomponent.hpp"

MapInstance::MapInstance(const Ndk::EntityHandle& e, TilesetCore* tcore, TilesetCore* ftcore)
    : m_entity(e), m_tilesetCore(tcore), m_fightTilesetCore(ftcore)
{
    m_mat = Nz::Material::New("Translucent2D");

    m_mat->EnableFaceCulling(true);
    m_mat->SetFaceFilling(Nz::FaceFilling_Fill);

    m_fightMat = Nz::Material::New("Translucent2D");

    m_fightMat->EnableFaceCulling(true);
    m_fightMat->SetFaceFilling(Nz::FaceFilling_Fill);

    m_tilemap = Nz::TileMap::New(Nz::Vector2ui { Def::MAPX + 1, Def::MAPY + 2 }, Nz::Vector2f { static_cast<float>(Def::TILEXSIZE), static_cast<float>(Def::TILEYSIZE) });
    m_tilemap->EnableIsometricMode(true);

    if (!m_entity->HasComponent<Ndk::NodeComponent>())
        m_entity->AddComponent<Ndk::NodeComponent>();

    if (!m_entity->HasComponent<Ndk::GraphicsComponent>())
        m_entity->AddComponent<Ndk::GraphicsComponent>();

    auto& graphicsComponent = m_entity->GetComponent<Ndk::GraphicsComponent>();
    graphicsComponent.Attach(m_tilemap, Def::MAP_LAYER);
}

void MapInstance::update()
{
    TealAssert(m_tilesetCore, "TilesetCore nullptr !");
    TealAssert(m_fightTilesetCore, "Fight TilesetCore nullptr !");
    TealAssert(m_map, "Map is not valid !");

    TilesetCore* tcore = (m_fightMode ? m_fightTilesetCore : m_tilesetCore);

    Nz::MaterialRef material = (m_fightMode ? m_fightMat : m_mat);
    m_tilemap->SetMaterial(0, material);

    for (unsigned i {}; i < Def::TILEARRAYSIZE; ++i)
    {
        auto& tile = m_map->tile(i);
        auto  pos = IndexToXY(i);
        Nz::Vector2ui tilePos { pos.first, pos.second };

        if (tile.visible)
        {
            Nz::Rectui tileRect { tcore->get(m_fightMode ? tile.fightTextureId : tile.textureId) * Def::TILEXSIZE, 0u, Def::TILEXSIZE, Def::TILEYSIZE };
            m_tilemap->EnableTile(tilePos, tileRect);
        }

        else
            m_tilemap->DisableTile(tilePos);
    }
}

bool MapInstance::adjacentPassable(unsigned sX, unsigned sY, unsigned eX, unsigned eY) // COORDFIX_REDO ?
{
    TealAssert(m_map, "Map is not valid !");

    if (!isPositionValid({ eX, eY }))
        return false;

    // Step 1.
    {
        const std::array<int, 8u>* mapDistanceX = isLineEven(sY) ? &Def::MAP_DISTANCE_EVEN_X : &Def::MAP_DISTANCE_UNEVEN_X; // ISLINEVEN_CRASH
        const std::array<int, 8u>* mapDistanceY = isLineEven(sY) ? &Def::MAP_DISTANCE_EVEN_Y : &Def::MAP_DISTANCE_UNEVEN_Y;
        bool found { false };

        for (unsigned i {}; i < Def::MAP_DISTANCE_COST.size(); ++i) // bug around there
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
        unsigned const tile = XYToIndex(eX, eY);
        auto& tiledata = m_map->tile(tile);

        return tiledata.obstacle == 0 && !tiledata.occupied;
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

    bool even = isLineEven(y); // ISLINEVEN_CRASH

    for (std::size_t i {}; i < Def::MAP_DISTANCE_COST.size(); ++i)
    {
        int newX = x + (even ? Def::MAP_DISTANCE_EVEN_X[i] : Def::MAP_DISTANCE_UNEVEN_X[i]);
        int newY = y + (even ? Def::MAP_DISTANCE_EVEN_Y[i] : Def::MAP_DISTANCE_UNEVEN_Y[i]);

        if (adjacentPassable(x, y, newX, newY))
        {
            micropather::StateCost nodeCost = { XYToNode(newX, newY), Def::MAP_DISTANCE_COST[i] };
            neighbors->push_back(nodeCost);
        }
    }
}
