// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/common/mapcomponent.hpp"

MapInstance::MapInstance(const Ndk::EntityHandle& e) : m_entity(e)
{
    m_mat = Nz::Material::New("Translucent2D");

    m_mat->EnableFaceCulling(true);
    m_mat->SetFaceFilling(Nz::FaceFilling_Fill);

    m_model = Nz::Model::New();
    m_model->SetMaterial(0, m_mat);

    if (!m_entity->HasComponent<Ndk::NodeComponent>()) //crash
        m_entity->AddComponent<Ndk::NodeComponent>();

    if (!m_entity->HasComponent<Ndk::GraphicsComponent>())
        m_entity->AddComponent<Ndk::GraphicsComponent>();

    auto& graphicsComponent = m_entity->GetComponent<Ndk::GraphicsComponent>();
    graphicsComponent.Attach(m_model, Def::MAP_LAYER);
}

MapInstance::MapInstance(const MapData& data, const Ndk::EntityHandle& e)
    : MapInstance(e)
{
    map = data.map;
    obs = data.obs;

    if (!m_mat->SetDiffuseMap(data.tileset))
        NazaraError("Error: Map Material SetDiffuseMap failed !");

    update();
}

bool MapInstance::update() // Thanks Lynix for this code
{
    Nz::MeshRef mesh = Nz::Mesh::New();
    mesh->CreateStatic();

    constexpr unsigned int width = Def::MAPX;
    constexpr unsigned int height = Def::MAPY;

    Nz::Vector2f tileSize { static_cast<float>(Def::TILEXSIZE), static_cast<float>(Def::TILEYSIZE) };

    unsigned int indexCount = width * height * 6;
    unsigned int vertexCount = width * height * 4;

    Nz::IndexBufferRef indexBuffer = Nz::IndexBuffer::New(vertexCount > std::numeric_limits<Nz::UInt16>::max(), indexCount, 
                                                          Nz::DataStorage_Hardware, Nz::BufferUsage_Static);
    Nz::VertexBufferRef vertexBuffer = Nz::VertexBuffer::New(Nz::VertexDeclaration::Get(Nz::VertexLayout_XY_UV), vertexCount, 
                                                             Nz::DataStorage_Hardware, Nz::BufferUsage_Static);

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

                if (x % 2 == 0)
                {
                    if (x == 0)
                    {
                        posPtr[0].Set((x + 0) * tileSize.x, (y + 0) * tileSize.y);
                        posPtr[1].Set((x + 1) * tileSize.x, (y + 0) * tileSize.y);
                        posPtr[2].Set((x + 1) * tileSize.x, (y + 1) * tileSize.y);
                        posPtr[3].Set((x + 0) * tileSize.x, (y + 1) * tileSize.y);
                    }

                    else
                    {
                        posPtr[0].Set((x / 2.f + 0) * tileSize.x, (y + 0) * tileSize.y);
                        posPtr[1].Set((x / 2.f + 1) * tileSize.x, (y + 0) * tileSize.y);
                        posPtr[2].Set((x / 2.f + 1) * tileSize.x, (y + 1) * tileSize.y);
                        posPtr[3].Set((x / 2.f + 0) * tileSize.x, (y + 1) * tileSize.y);
                    }
                }

                else
                {
                    posPtr[0].Set((x + 0) * tileSize.x - x * tileSize.x / 2.f, ((y + 0) * tileSize.y) + tileSize.y / 2.f);
                    posPtr[1].Set((x + 1) * tileSize.x - x * tileSize.x / 2.f, ((y + 0) * tileSize.y) + tileSize.y / 2.f);
                    posPtr[2].Set((x + 1) * tileSize.x - x * tileSize.x / 2.f, ((y + 1) * tileSize.y) + tileSize.y / 2.f);
                    posPtr[3].Set((x + 0) * tileSize.x - x * tileSize.x / 2.f, ((y + 1) * tileSize.y) + tileSize.y / 2.f);
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

                unsigned tileNumber = map[x + y * width];

                float textureX = 64.f * tileNumber;

                texCoordsPtr[0].Set((textureX + 0.f ) / 256.f, 0.f);
                texCoordsPtr[1].Set((textureX + 64.f) / 256.f, 0.f);
                texCoordsPtr[2].Set((textureX + 64.f) / 256.f, 1.f);
                texCoordsPtr[3].Set((textureX + 0.f ) / 256.f, 1.f);
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
    //subMesh->GenerateAABB();

    mesh->AddSubMesh(subMesh);
    //mesh->SetMaterialCount(1);

    m_model->SetMesh(mesh);
    return true;
}

void MapInstance::NodeToXY(void* node, unsigned& x, unsigned& y)
{
    int index = (int) node;
    auto xy = IndexToXY(static_cast<unsigned>(index));

    x = xy.first;
    y = xy.second;
}

void* MapInstance::XYToNode(unsigned x, unsigned y)
{
    return (void*) (y * Def::MAPX + x);
}

void MapInstance::XYToArray(unsigned /*x*/, unsigned& y)
{
    y /= 2;
}

std::pair<unsigned, unsigned> MapInstance::IndexToXY(unsigned index)
{
    unsigned x {}, y {};

    y = index / Def::MAPX;
    x = index - y * Def::MAPX;

    return std::make_pair(x, y);
}

bool MapInstance::passable(unsigned sX, unsigned sY, unsigned eX, unsigned eY)
{
    //Step 1.
    {
        if ((sX == eX - 2 && sY == eY)
            || (sX == eX + 2 && sY == eY)
            || (sX == eX && sY == eY - 2)
            || (sX == eX && sY == eY + 2)
            //diagonals
            || (sX == eX + 1 && sY == eY + 1)
            || (sX == eX - 1 && sY == eY - 1)
            || (sX == eX + 1 && sY == eY - 1)
            || (sX == eX - 1 && sY == eY + 1))
            ; //ok, continue
        else
            return false;
    }

    //Step 2.
    {
        if (eX > Def::LMAPX || eY > Def::LMAPY)
            return false;

        XYToArray(eX, eY);

        if (eX > Def::MAPX || eY > Def::MAPY)
            return false;

        unsigned const tile { eX + eY*Def::MAPX };

        unsigned const tileNumber = obs[tile];
        return tileNumber == 0;
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
    NazaraAssert(neighbors, "Micropather neighbors null !");

    static constexpr std::array<int, 8> dx { 0,   2,   0 , -2,   1,   -1,    1,   -1 };
    static constexpr std::array<int, 8> dy { 2,   0,  -2,   0,   1,   -1,   -1,    1 };
    static constexpr std::array<float, 8> cost { 2.f, 2.f, 2.f, 2.f, 1.5f, 1.5f, 1.5f, 1.5f };

    unsigned x {}, y {};
    NodeToXY(node, x, y);

    for (std::size_t i {}; i < cost.size(); ++i)
    {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (passable(x, y, newX, newY))
        {
            micropather::StateCost nodeCost = { XYToNode(newX, newY), cost[i] };
            neighbors->push_back(nodeCost);
        }
    }
}
