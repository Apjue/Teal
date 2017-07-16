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

std::pair<unsigned, unsigned> IndexToXY(unsigned index) // COORDFIX_REDO
{
    unsigned evenLines {};
    unsigned unevenLines {};

    for (unsigned i {}; i < index; ++i)
    {
        unsigned num = evenLines * Def::MAPX + unevenLines * (Def::MAPX - 1u);
        num += (i - num);

        if (evenLines == unevenLines && num % Def::MAPX == 0)
            ++evenLines;

        else if (evenLines > unevenLines && num % (Def::MAPX - 1u) == 0)
            ++unevenLines;
    }

    index -= evenLines * Def::MAPX;
    index -= unevenLines * (Def::MAPX - 1u);
    
    unsigned x { index };
    unsigned y { unevenLines + evenLines };

    return std::make_pair(x, y);
}

unsigned XYToIndex(unsigned x, unsigned y)
{
    unsigned result { x };
    bool even { true };

    for (unsigned i {}; i < y; ++i)
    {
        if (even)
        {
            result += Def::MAPX;
            even = false;
        }

        else
        {
            result += (Def::MAPX - 1u);
            even = true;
        }
    }

    return result;
}
