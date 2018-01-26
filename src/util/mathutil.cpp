// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/mathutil.hpp"

bool interactSegment(const Vector2fPair& ab, const Vector2fPair& ij, Nz::Vector2f* interactPoint) // todo: understand this function later.
{
    Nz::Vector2f abVec(ab.second - ab.first), ijVec(ij.second - ij.first);
    float denom(abVec.x * ijVec.y - abVec.y * ijVec.x);

    if (denom == 0)
        return false;

    float u { (ab.first.x * ijVec.y - ij.first.x * ijVec.y - ijVec.x * ab.first.y + ijVec.x * ij.first.y) / denom };
    float v { (-abVec.x * ab.first.y + abVec.x * ij.first.y + abVec.y * ab.first.x - abVec.y * ij.first.x) / denom };

    if (u >= -1 && u <= 0 && v >= -1 && v <= 0)
    {
        if (interactPoint != nullptr)
            *interactPoint = ab.first - abVec * u;

        return true;
    }

    return false;
}
