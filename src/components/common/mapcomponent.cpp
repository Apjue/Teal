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
    TealAssert(m_fightTilesetCore, "Fight TilesetCore nullptr !");
    TealAssert(m_map, "Map is not valid !");

    Nz::MeshRef mesh = Nz::Mesh::New();
    mesh->CreateStatic();

    constexpr unsigned int width = Def::ARRAYMAPX; // COORDFIX_REDO
    constexpr unsigned int height = Def::ARRAYMAPY;

    Nz::Vector2f tileSize { static_cast<float>(Def::TILEXSIZE), static_cast<float>(Def::TILEYSIZE) };

    unsigned int indexCount = width * height * 6;
    unsigned int vertexCount = width * height * 4;

    Nz::IndexBufferRef indexBuffer = Nz::IndexBuffer::New(vertexCount > std::numeric_limits<Nz::UInt16>::max(), indexCount, Nz::DataStorage_Hardware, 0);
    Nz::VertexBufferRef vertexBuffer = Nz::VertexBuffer::New(Nz::VertexDeclaration::Get(Nz::VertexLayout_XY_UV), vertexCount, Nz::DataStorage_Hardware, 0);

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

                unsigned realY = (y == 0 ? y : y / 2u);
                
                if (isLineEven(y))
                {
                    posPtr[0].Set((x + 0) * tileSize.x, (realY + 0) * tileSize.y);
                    posPtr[1].Set((x + 1) * tileSize.x, (realY + 0) * tileSize.y);
                    posPtr[2].Set((x + 1) * tileSize.x, (realY + 1) * tileSize.y);
                    posPtr[3].Set((x + 0) * tileSize.x, (realY + 1) * tileSize.y);
                }

                else
                {
                    posPtr[0].Set((x + 0) * tileSize.x + tileSize.x / 2, (realY + 0) * tileSize.y + Def::TILEYSIZE / 2);
                    posPtr[1].Set((x + 1) * tileSize.x + tileSize.x / 2, (realY + 0) * tileSize.y + Def::TILEYSIZE / 2);
                    posPtr[2].Set((x + 1) * tileSize.x + tileSize.x / 2, (realY + 1) * tileSize.y + Def::TILEYSIZE / 2);
                    posPtr[3].Set((x + 0) * tileSize.x + tileSize.x / 2, (realY + 1) * tileSize.y + Def::TILEYSIZE / 2);
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


                const TileData& tile = m_map->tile(XYToIndex(x, y)); /// \todo Invisible tile
                unsigned tileNumber = m_fightMode ? m_fightTilesetCore->get(tile.fightTextureId) : m_tilesetCore->get(tile.textureId);

                float textureX = tileSize.x * tileNumber;
                float tilesetSize = m_fightMode ? Def::FIGHTTILESETSIZE : Def::TILESETSIZE;

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

    subMesh->SetAABB(Nz::Boxf (0.f, 0.f, 0.f, width * tileSize.x, height * tileSize.y, 0.f));
    subMesh->GenerateAABB();

    mesh->AddSubMesh(subMesh);

    Nz::MaterialRef material = m_fightMode ? m_fightMat : m_mat;

    m_model->SetMesh(mesh);
    m_model->SetMaterial(0, material);

    return true;
}

bool MapInstance::adjacentPassable(unsigned sX, unsigned sY, unsigned eX, unsigned eY) // COORDFIX_REDO ?
{
    TealAssert(m_map, "Map is not valid !");

    // Step 1.
    {
        bool even = isLineEven(sY);

        for (unsigned i {}; i < Def::MAP_DISTANCE_COST.size(); ++i)
        {
            if (IndexToXY(XYToIndex(eX, eY)).second != sY
            && (even ? Def::MAP_DISTANCE_EVEN_Y[i] : Def::MAP_DISTANCE_UNEVEN_Y[i]) == 0)
                return false;
        }
    }

    // Step 2.
    {
        if (!isPositionValid({ eX, eY }))
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

    bool even = isLineEven(y);

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
