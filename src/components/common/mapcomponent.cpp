// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/common/mapcomponent.hpp"

bool MapComponent::passable(unsigned sX, unsigned sY, unsigned eX, unsigned eY)
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

float MapComponent::LeastCostEstimate(void* nodeStart, void* nodeEnd)
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

void MapComponent::AdjacentCost(void* node, std::vector<micropather::StateCost>* neighbors)
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
