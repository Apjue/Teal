// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

void NodeToXY(void* node, unsigned& x, unsigned& y)
{
    int index {};
    index = static_cast<int>(reinterpret_cast<std::intptr_t>(node));
    auto xy = IndexToXY(static_cast<unsigned>(index));

    x = xy.first;
    y = xy.second;
}

void* XYToNode(unsigned x, unsigned y)
{
    std::size_t result = static_cast<std::size_t>(XYToIndex(x, y));
    return reinterpret_cast<void*>(result);
}

void XYToArray(unsigned& x, unsigned& y)
{
    unsigned tmpX = x;
    x /= 2;

    if (tmpX % 2 != 0)
        x += 8u;

    y /= 2;
}

std::pair<unsigned, unsigned> IndexToXY(unsigned index)
{
    unsigned x {}, y {};

    y = index / Def::MAPX;
    x = index - y * Def::MAPX;

    return std::make_pair(x, y);
}

unsigned XYToIndex(unsigned x, unsigned y)
{
    return x + y * Def::MAPX;
}
