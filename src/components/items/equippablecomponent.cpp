// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <string>
#include "cache/doublestore.hpp"
#include "components/items/equippablecomponent.hpp"

namespace Nz
{

unsigned int LuaImplQueryArg(const LuaState& state, int index, EquippableComponent* component, TypeTag<EquippableComponent>)
{
    state.CheckType(index, Nz::LuaType_Table);

    component->bodypart = EquippableComponent::stringToBodypart(state.CheckField<Nz::String>("bodypart", index));
    component->side = EquippableComponent::stringToSide(state.CheckField<Nz::String>("side", index));

    if (state.GetField("skill_id", index) == Nz::LuaType_String)
    {
        const char* skillName = state.CheckString(-1);
        component->attackId = DoubleStores<SkillData>::getInstance()->getItemIndex(skillName);
    }

    state.Pop();

    return 1;
}

int LuaImplReplyVal(const LuaState& state, EquippableComponentHandle&& component, TypeTag<EquippableComponentHandle>)
{
    state.PushTable();
    {
        state.PushField<Nz::String>("bodypart", EquippableComponent::bodypartToString(component->bodypart));
        state.PushField<Nz::String>("side", EquippableComponent::sideToString(component->side));

        if (component->attackId != SkillStore::InvalidID)
            state.PushField<std::string>("skill_id", DoubleStores<SkillData>::getInstance()->getItem(component->attackId).codename);
    }

    return 1;
}

} // namespace Nz


EquippableComponent::BodyPart EquippableComponent::stringToBodypart(Nz::String string)
{
    string = string.ToLower();

    if (string == "head")
        return Head;

    if (string == "neck")
        return Neck;

    if (string == "arms")
        return Arms;

    if (string == "hands")
        return Hands;

    if (string == "wrists")
        return Wrists;

    if (string == "digits")
        return Digits;

    if (string == "chest")
        return Chest;

    if (string == "hip")
        return Hip;

    if (string == "legs")
        return Legs;

    if (string == "feet")
        return Feet;

    throw std::runtime_error { "Invalid bodypart" };
}

const char* EquippableComponent::bodypartToString(BodyPart bodypart)
{
    switch (bodypart)
    {
        case EquippableComponent::Head:
            return "head";

        case EquippableComponent::Neck:
            return "neck";

        case EquippableComponent::Arms:
            return "arms";

        case EquippableComponent::Hands:
            return "hands";

        case EquippableComponent::Wrists:
            return "wrists";

        case EquippableComponent::Digits:
            return "digits";

        case EquippableComponent::Chest:
            return "chest";

        case EquippableComponent::Hip:
            return "hip";

        case EquippableComponent::Legs:
            return "legs";

        case EquippableComponent::Feet:
            return "feet";
    }

    throw std::runtime_error { "Invalid bodypart" };
}

EquippableComponent::Side EquippableComponent::stringToSide(Nz::String string)
{
    string = string.ToLower();

    if (string == "both")
        return Both;

    if (string == "right")
        return Right;

    if (string == "left")
        return Left;

    throw std::runtime_error { "Invalid side" };
}

const char* EquippableComponent::sideToString(Side side)
{
    switch (side)
    {
        case EquippableComponent::Both:
            return "both";

        case EquippableComponent::Right:
            return "right";

        case EquippableComponent::Left:
            return "left";
    }

    throw std::runtime_error { "Invalid side" };
}
