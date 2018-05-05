// Copyright (C) 2018 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef DOUBLESTORE_HPP
#define DOUBLESTORE_HPP

#include <Nazara/Core/String.hpp>
#include <unordered_map>
#include <vector>
#include <memory>
#include <limits>
#include "util/assert.hpp"
#include "data/skilldata.hpp"
#include "data/statemetadata.hpp"
#include "data/animationdata.hpp"

template<class Value, class LightID = std::size_t, class HeavyID = Nz::String>
class DoubleStore;

using SkillStore = DoubleStore<SkillData>;
using StateMDStore = DoubleStore<StateMetaData>;
using AnimationStore = DoubleStore<AnimationData>;


namespace Ndk { class World; }
namespace Nz  { class RenderWindow; }
struct GameData;

template<class Value, class LightID = std::size_t, class HeavyID = Nz::String>
class DoubleStores // Singleton
{
public:
    ~DoubleStores() = default;

    static inline const std::shared_ptr<DoubleStore<Value, LightID, HeavyID>> getInstance()
    {
        return instance;
    }

private:
    static std::shared_ptr<DoubleStore<Value, LightID, HeavyID>> instance;

    DoubleStores() = default;
    friend void initializeTeal(Ndk::World& world, Nz::RenderWindow& window, GameData& data); // find a cleaner way?
};

template<class Value, class LightID, class HeavyID>
std::shared_ptr<DoubleStore<Value, LightID, HeavyID>> DoubleStores<Value, LightID, HeavyID>::instance {};

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

    inline bool hasItem(const HeavyID& name);
    inline bool hasItem(LightID id);
    inline LightID addItem(const HeavyID& name, const Value& skill);

    inline Value& getItem(LightID id);
    inline const Value& getItem(LightID id) const;

    inline Value& getItem(const HeavyID& name);
    inline const Value& getItem(const HeavyID& name) const;

    inline LightID getItemIndex(const HeavyID& name) const;

    static constexpr LightID InvalidID { std::numeric_limits<LightID>().max() }; // Use this value for the "No Attack Chosen" thing

private:
    LightID m_counter {};
    std::vector<Value> m_items;
    std::unordered_map<HeavyID, LightID> m_conversionTable;
};

#include "doublestore.inl"

#endif // DOUBLESTORE_HPP
