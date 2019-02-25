// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "equipmentdata.hpp"

BodyPart stringToBodypart(Nz::String string)
{
    string = string.ToLower();

    if (string == "head")
        return BodyPart::Head;

    if (string == "neck")
        return BodyPart::Neck;

    if (string == "arms")
        return BodyPart::Arms;

    if (string == "hands")
        return BodyPart::Hands;

    if (string == "wrists")
        return BodyPart::Wrists;

    if (string == "digits")
        return BodyPart::Digits;

    if (string == "chest")
        return BodyPart::Chest;

    if (string == "hip")
        return BodyPart::Hip;

    if (string == "legs")
        return BodyPart::Legs;

    if (string == "feet")
        return BodyPart::Feet;

    throw std::runtime_error { "Invalid bodypart" };
}

const char* bodypartToString(BodyPart bodypart)
{
    switch (bodypart)
    {
        case BodyPart::Head:
            return "head";

        case BodyPart::Neck:
            return "neck";

        case BodyPart::Arms:
            return "arms";

        case BodyPart::Hands:
            return "hands";

        case BodyPart::Wrists:
            return "wrists";

        case BodyPart::Digits:
            return "digits";

        case BodyPart::Chest:
            return "chest";

        case BodyPart::Hip:
            return "hip";

        case BodyPart::Legs:
            return "legs";

        case BodyPart::Feet:
            return "feet";
    }

    throw std::runtime_error { "Invalid bodypart" };
}

Side stringToSide(Nz::String string)
{
    string = string.ToLower();

    if (string == "both")
        return Side::Both;

    if (string == "right")
        return Side::Right;

    if (string == "left")
        return Side::Left;

    throw std::runtime_error { "Invalid side" };
}

const char* sideToString(Side side)
{
    switch (side)
    {
        case Side::Both:
            return "both";

        case Side::Right:
            return "right";

        case Side::Left:
            return "left";
    }

    throw std::runtime_error { "Invalid side" };
}
