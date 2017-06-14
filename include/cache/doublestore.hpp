// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef DOUBLESTORE_HPP
#define DOUBLESTORE_HPP

#include <Nazara/Core/String.hpp>
#include <unordered_map>
#include <vector>
#include "data/skilldata.hpp"

template<class Value, class LightID = std::size_t, class HeavyID = Nz::String>
class DoubleStore;

using SkillStore = DoubleStore<SkillData>;
//using StateStore = DoubleStore<StateData>;

///
/// \class DoubleStore
///
/// \brief Store items with 2 IDs.
///        LightID should be preferred over HeavyID
///

template<class Value, class LightID, class HeavyID>
class DoubleStore
{
public:
    using LightId = LightID;
    using HeavyId = HeavyID;

    DoubleStore() = default;
    ~DoubleStore() = default;

    inline LightID addSkill(const HeavyID& name, const Value& skill);
    inline Value&  getSkill(LightID id);
    inline LightID getSkillIndex(const HeavyID& name);

private:
    LightID m_counter;
    std::vector<Value> m_items;
    std::unordered_map<HeavyID, LightID> m_conversionTable;
};

#include "doublestore.inl"

#endif // DOUBLESTORE_HPP
