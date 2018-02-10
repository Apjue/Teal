// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/mathutil.hpp"
#include "util/mapposutil.hpp"

Vector2fPair getTileCornerSegment(Orientation corner, unsigned x, unsigned y)
{
    Nz::Rectf aabb = getTileAABB(x, y);

    switch (corner)
    {
        case Orientation::Down:
            return { Nz::Vector2f { aabb.x, aabb.y + aabb.height },
                Nz::Vector2f { aabb.x + aabb.width, aabb.y + aabb.height } };

        case Orientation::DownRight:
            return  { Nz::Vector2f { aabb.x + aabb.width / 2, aabb.y + aabb.height },
                Nz::Vector2f { aabb.x + aabb.width, aabb.y + aabb.height / 2 } };

        case Orientation::Right:
            return  { Nz::Vector2f { aabb.x + aabb.width, aabb.y + aabb.height },
                Nz::Vector2f { aabb.x + aabb.width, aabb.y } };

        case Orientation::UpRight:
            return  { Nz::Vector2f { aabb.x + aabb.width, aabb.y + aabb.height / 2 },
                Nz::Vector2f { aabb.x + aabb.width / 2, aabb.y } };

        case Orientation::Up:
            return  { Nz::Vector2f { aabb.x + aabb.width, aabb.y },
                Nz::Vector2f { aabb.x, aabb.y } };

        case Orientation::UpLeft:
            return  { Nz::Vector2f { aabb.x + aabb.width / 2, aabb.y },
                Nz::Vector2f { aabb.x, aabb.y + aabb.height / 2 } };

        case Orientation::Left:
            return  { Nz::Vector2f { aabb.x, aabb.y },
                Nz::Vector2f { aabb.x, aabb.y + aabb.height } };

        case Orientation::DownLeft:
            return  { Nz::Vector2f { aabb.x, aabb.y + aabb.height / 2 },
                Nz::Vector2f { aabb.x + aabb.width / 2, aabb.y + aabb.height } };
    }

    throw std::runtime_error { "Corner not initialized properly" };
}

Vector2fTriplet getTileOutterCorners(const AbsTile& from, const AbsTile& to)
{
    Vector2fTriplet rays { getTileCenter(from) };
    bool extremity1filled { false };

    for (unsigned i {}; i < Def::TileVertexNumber; ++i) // Detect correct extremities
    {
        bool isExtremity { true };
        Nz::Ternary left = Nz::Ternary_Unknown; // is this ray at the left ?

        for (unsigned j {}; j < Def::TileVertexNumber; ++j)
        {
            if (j == i)
                continue;

            Direction dir = static_cast<Direction>(j);
            bool atLeft = isLeft(rays.first, getTileVertex(static_cast<Direction>(i), to.x, to.y), getTileVertex(dir, to.x, to.y));

            if (left == Nz::Ternary_Unknown)
                left = atLeft ? Nz::Ternary_True : Nz::Ternary_False;

            if (left != (atLeft ? Nz::Ternary_True : Nz::Ternary_False))
            {
                isExtremity = false;
                break;
            }
        }

        if (isExtremity)
        {
            if (!extremity1filled)
            {
                rays.second = getTileVertex(static_cast<Direction>(i), to.x, to.y);
                extremity1filled = true;
            }

            else
            {
                rays.third = getTileVertex(static_cast<Direction>(i), to.x, to.y);
                break;
            }
        }
    }

    TealAssert(rays.second != Nz::Vector2f {} && rays.third != Nz::Vector2f {}, "Triangle not completely filled");

    // Swap triangle's vertex: rays.second must be at the left, rays.third must be at the right
    if (!isRight(rays.first, rays.second, rays.third))
        std::swap(rays.second, rays.third);

    return rays;
}

Nz::Vector2f getTileVertex(Direction vertex, unsigned x, unsigned y)
{
    Nz::Rectf aabb = getTileAABB(x, y);

    switch (vertex)
    {
        case Direction::Up:
            return { aabb.x + aabb.width / 2, aabb.y };

        case Direction::Down:
            return { aabb.x + aabb.width / 2, aabb.y + aabb.height };

        case Direction::Left:
            return { aabb.x, aabb.y + aabb.height / 2 };

        case Direction::Right:
            return { aabb.x + aabb.width, aabb.y + aabb.height / 2 };
    }

    throw std::runtime_error { "Initialize your arguments next time..." };
}

