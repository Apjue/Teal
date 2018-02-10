// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

void NodeToXY(void* node, unsigned& x, unsigned& y)
{
    int index {};
    index = int(reinterpret_cast<std::intptr_t>(node));
    auto xy = IndexToXY(unsigned(index));

    x = xy.first;
    y = xy.second;

    TealAssert(isPositionValid({ x, y }), "Invalid position !");
}

void* XYToNode(unsigned x, unsigned y)
{
    TealAssert(isPositionValid({ x, y }), "Invalid position !");

    std::size_t result = static_cast<std::size_t>(XYToIndex(x, y));
    return reinterpret_cast<void*>(result);
}

std::pair<unsigned, unsigned> IndexToXY(unsigned index)
{
    TealAssert(index < Def::TileArraySize, "Invalid index !");

    unsigned evenLines {};
    unsigned unevenLines {};

    for (unsigned i {}; i <= index; ++i)
    {
        unsigned num = evenLines * Def::MapX + unevenLines * (Def::MapX - 1u);
        
        if (evenLines == unevenLines && (i - num) > Def::MapX)
        {
            ++evenLines;
            --index;
        }

        else if (evenLines > unevenLines && (i - num) > (Def::MapX - 1u))
        {
            ++unevenLines;
            --index;
        }
    }

    index -= evenLines * Def::MapX;
    index -= unevenLines * (Def::MapX - 1u);
    
    unsigned x { index };
    unsigned y { unevenLines + evenLines };

    return std::make_pair(x, y);
}

unsigned XYToIndex(unsigned x, unsigned y)
{
    TealAssert(isPositionValid({ x, y }), "Invalid position !");

    unsigned result { x };
    bool even { true };

    for (unsigned i {}; i < y; ++i)
    {
        if (even)
        {
            result += Def::ArrayMapX;
            even = false;
        }

        else
        {
            result += Def::MapX;
            even = true;
        }
    }

    return result;
}

std::pair<unsigned, unsigned> countLines(unsigned y)
{
    TealAssert(y <= Def::ArrayMapY, "Invalid y !");

    unsigned evenLines {};
    unsigned unevenLines {};
    auto index = XYToIndex(0u, y);

    for (unsigned i {}; i < index; ++i)
    {
        unsigned num = evenLines * Def::MapX + unevenLines * (Def::MapX - 1u);

        if (evenLines == unevenLines && (i - num) > Def::MapX)
        {
            ++evenLines;
            --index;
        }

        else if (evenLines > unevenLines && (i - num) > (Def::MapX - 1u))
        {
            ++unevenLines;
            --index;
        }
    }

    return std::make_pair(evenLines, unevenLines);
}

unsigned distanceBetweenTiles(const AbsTile& a, const AbsTile& b)
{
    Nz::Vector2f aCenter = getTileCenter(a);
    Nz::Vector2f bCenter = getTileCenter(b);

    Nz::Vector2ui graphicalDistance { unsigned (std::abs(aCenter.x - bCenter.x)), unsigned (std::abs(aCenter.y - bCenter.y)) };
    unsigned distance = unsigned(std::floor(graphicalDistance.x / Def::TileSizeX) + std::floor(graphicalDistance.y / Def::TileSizeY));

    if (graphicalDistance.x % Def::TileSizeX == Def::TileSizeX / 2 && graphicalDistance.y % Def::TileSizeY == Def::TileSizeY / 2)
        ++distance;

    return distance;
}

float graphicalDistanceBetweenTiles(const AbsTile& a, const AbsTile& b)
{
    return getTileCenter(a).Distancef(getTileCenter(b));
}


Nz::Rectf getTileAABB(const AbsTile& tile)
{
    return getTileAABB(tile.x, tile.y);
}

Nz::Rectf getTileAABB(unsigned x, unsigned y)
{
    return Nz::Rectf { float(x * Def::TileSizeX + (isLineEven(y) ? 0u : Def::TileSizeX / 2)), float((y * Def::TileSizeY) / 2), float(Def::TileSizeX), float(Def::TileSizeY) };
}

Nz::Vector2f getTileCenter(const AbsTile& tile)
{
    return getTileCenter(tile.x, tile.y);
}

Nz::Vector2f getTileCenter(unsigned x, unsigned y)
{
    Nz::Rectf aabb = getTileAABB(x, y);
    return { aabb.x + aabb.width / 2, aabb.y + aabb.height / 2 };
}


Vector2fPair getTileCornerSegment(Orientation corner, const AbsTile& tile)
{
    return getTileCornerSegment(corner, tile.x, tile.y);
}

Nz::Vector2f getTileVertex(Direction vertex, const AbsTile& tile)
{
    return getTileVertex(vertex, tile.x, tile.y);
}
