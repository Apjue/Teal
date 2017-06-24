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

    m_model = Nz::Model::New();

    if (!m_entity->HasComponent<Ndk::NodeComponent>())
        m_entity->AddComponent<Ndk::NodeComponent>();

    if (!m_entity->HasComponent<Ndk::GraphicsComponent>())
        m_entity->AddComponent<Ndk::GraphicsComponent>();

    auto& graphicsComponent = m_entity->GetComponent<Ndk::GraphicsComponent>();
    graphicsComponent.Attach(m_model, Def::MAP_LAYER);
}

bool MapInstance::update() // Thanks Lynix for this code
{
    TealAssert(m_tilesetCore, "TilesetCore nullptr !");
    TealAssert(m_tilesetCore, "Fight TilesetCore nullptr !");
    TealAssert(m_map, "Map is not valid !");

    Nz::MeshRef mesh = Nz::Mesh::New();
    mesh->CreateStatic();

    constexpr unsigned int width = Def::MAPX;
    constexpr unsigned int height = Def::MAPY;

    Nz::Vector2f tileSize { static_cast<float>(Def::TILEXSIZE), static_cast<float>(Def::TILEYSIZE) };

    unsigned int indexCount = width * height * 6;
    unsigned int vertexCount = width * height * 4;

    Nz::IndexBufferRef indexBuffer = Nz::IndexBuffer::New(vertexCount > std::numeric_limits<Nz::UInt16>::max(), indexCount,
                                                          Nz::DataStorage_Hardware, 0);
    Nz::VertexBufferRef vertexBuffer = Nz::VertexBuffer::New(Nz::VertexDeclaration::Get(Nz::VertexLayout_XY_UV), vertexCount,
                                                             Nz::DataStorage_Hardware, 0);

    {
        Nz::VertexMapper vertexMapper(vertexBuffer, Nz::BufferAccess_WriteOnly);

        auto positionPtr = vertexMapper.GetComponentPtr<Nz::Vector2f>(Nz::VertexComponent_Position);
        auto uvPtr = vertexMapper.GetComponentPtr<Nz::Vector2f>(Nz::VertexComponent_TexCoord);

        Nz::IndexMapper indexMapper(indexBuffer, Nz::BufferAccess_WriteOnly);

        for (unsigned i {}; i < indexCount; ++i)
            indexMapper.Set(i, i);

        for (unsigned x {}; x < width; ++x)
        {
            for (unsigned y {}; y < height; ++y)
            {
                unsigned index = (x + y * width) * 6;
                unsigned vertex = (x + y * width) * 4;

                auto posPtr = positionPtr + vertex;
                auto texCoordsPtr = uvPtr + vertex;


                if (x < Def::REALMAPX)
                {
                    posPtr[0].Set((x + 0) * tileSize.x, (y + 0) * tileSize.y);
                    posPtr[1].Set((x + 1) * tileSize.x, (y + 0) * tileSize.y);
                    posPtr[2].Set((x + 1) * tileSize.x, (y + 1) * tileSize.y);
                    posPtr[3].Set((x + 0) * tileSize.x, (y + 1) * tileSize.y);
                }

                else
                {
                    posPtr[0].Set((x + 0 - Def::REALMAPX) * tileSize.x + Def::TILEGXSIZE, (y + 0) * tileSize.y + tileSize.y / 2.f);
                    posPtr[1].Set((x + 1 - Def::REALMAPX) * tileSize.x + Def::TILEGXSIZE, (y + 0) * tileSize.y + tileSize.y / 2.f);
                    posPtr[2].Set((x + 1 - Def::REALMAPX) * tileSize.x + Def::TILEGXSIZE, (y + 1) * tileSize.y + tileSize.y / 2.f);
                    posPtr[3].Set((x + 0 - Def::REALMAPX) * tileSize.x + Def::TILEGXSIZE, (y + 1) * tileSize.y + tileSize.y / 2.f);
                }

                posPtr[0].y = -posPtr[0].y;
                posPtr[1].y = -posPtr[1].y;
                posPtr[2].y = -posPtr[2].y;
                posPtr[3].y = -posPtr[3].y;

                indexMapper.Set(index + 0, vertex + 0);
                indexMapper.Set(index + 1, vertex + 2);
                indexMapper.Set(index + 2, vertex + 1);
                indexMapper.Set(index + 3, vertex + 3);
                indexMapper.Set(index + 4, vertex + 2);
                indexMapper.Set(index + 5, vertex + 0);

                auto tileName = m_map->tile(XYToIndex(x, y)).textureId;
                unsigned tileNumber = m_tilesetCore->get(tileName);

                float textureX = tileSize.x * tileNumber;
                float tilesetSize = Def::TILESETSIZE; // m_fightMode ? Def::FIGHTTILESETSIZE : Def::TILESETSIZE;

                texCoordsPtr[0].Set((textureX + 0.f       ) / tilesetSize, 0.f);
                texCoordsPtr[1].Set((textureX + tileSize.x) / tilesetSize, 0.f);
                texCoordsPtr[2].Set((textureX + tileSize.x) / tilesetSize, 1.f);
                texCoordsPtr[3].Set((textureX + 0.f       ) / tilesetSize, 1.f);
            }
        }
    }

    Nz::StaticMeshRef subMesh = Nz::StaticMesh::New(mesh);
    if (!subMesh->Create(vertexBuffer))
    {
        NazaraError("Failed to create StaticMesh");
        return false;
    }

    subMesh->SetIndexBuffer(indexBuffer);

    subMesh->SetAABB(Nz::Boxf(0.f, 0.f, 0.f, width * tileSize.x, height * tileSize.y, 0.f));
    subMesh->GenerateAABB();

    mesh->AddSubMesh(subMesh);

    m_model->SetMesh(mesh);
    m_model->SetMaterial(0, m_mat);

    return true;
}

bool MapInstance::adjacentPassable(unsigned sX, unsigned sY, unsigned eX, unsigned eY)
{
    TealAssert(m_fightTilesetCore, "Fight TilesetCore nullptr !");
    TealAssert(m_map, "Map is not valid !");

    // Step 1.
    {
        if ((sX == eX - 2 && sY == eY)
         || (sX == eX + 2 && sY == eY)
         || (sX == eX && sY == eY - 2)
         || (sX == eX && sY == eY + 2)
            // Diagonals
         || (sX == eX + 1 && sY == eY + 1)
         || (sX == eX - 1 && sY == eY - 1)
         || (sX == eX + 1 && sY == eY - 1)
         || (sX == eX - 1 && sY == eY + 1))
            ; // Ok, continue
        else
            return false;
    }

    // Step 2.
    {
        if (eX > Def::LMAPX || eY > Def::LMAPY)
            return false;

        XYToArray(eX, eY);

        if (eX > Def::MAPX || eY > Def::MAPY)
            return false;

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

    for (std::size_t i {}; i < Def::MAP_DISTANCE_COST.size(); ++i)
    {
        int newX = x + Def::MAP_DISTANCE_X[i];
        int newY = y + Def::MAP_DISTANCE_Y[i];

        if (adjacentPassable(x, y, newX, newY))
        {
            micropather::StateCost nodeCost = { XYToNode(newX, newY), Def::MAP_DISTANCE_COST[i] };
            neighbors->push_back(nodeCost);
        }
    }
}
