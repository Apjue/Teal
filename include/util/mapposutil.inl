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

std::pair<unsigned, unsigned> IndexToXY(unsigned index)
{
    unsigned evenLines {};
    unsigned unevenLines {};

    for (unsigned i {}; i <= index; ++i)
    {
        unsigned num = evenLines * Def::MAPX + unevenLines * (Def::MAPX - 1u);
        
        if (evenLines == unevenLines && (i - num) > Def::MAPX)
        {
            ++evenLines;
            --index;
        }

        else if (evenLines > unevenLines && (i - num) > (Def::MAPX - 1u))
        {
            ++unevenLines;
            --index;
        }
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
            result += Def::ARRAYMAPX;
            even = false;
        }

        else
        {
            result += Def::MAPX;
            even = true;
        }
    }

    return result;
}

bool isLineEven(unsigned y)
{
    return y % 2 == 0;
}

std::pair<unsigned, unsigned> countLines(unsigned y)
{
    unsigned evenLines {};
    unsigned unevenLines {};
    auto index = XYToIndex(0u, y);

    for (unsigned i {}; i < index; ++i)
    {
        unsigned num = evenLines * Def::MAPX + unevenLines * (Def::MAPX - 1u);

        if (evenLines == unevenLines && (i - num) > Def::MAPX)
        {
            ++evenLines;
            --index;
        }

        else if (evenLines > unevenLines && (i - num) > (Def::MAPX - 1u))
        {
            ++unevenLines;
            --index;
        }
    }

    return std::make_pair(evenLines, unevenLines);
}
