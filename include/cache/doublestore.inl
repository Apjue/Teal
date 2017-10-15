// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

template<class Value, class LightID, class HeavyID>
auto DoubleStore<Value, LightID, HeavyID>::addItem(const HeavyID& name, const Value& skill) -> LightID
{
    TealAssert(m_conversionTable.find(name) == m_conversionTable.end(), "Item with that name already exists !");

    m_items.push_back(skill);
    m_conversionTable[name] = m_counter;

    return m_counter++;
}

template<class Value, class LightID, class HeavyID>
auto DoubleStore<Value, LightID, HeavyID>::getItem(LightID id) -> Value&
{
    return m_items[id];
}

template<class Value, class LightID, class HeavyID>
auto DoubleStore<Value, LightID, HeavyID>::getItem(LightID id) const -> const Value&
{
    return m_items[id];
}

template<class Value, class LightID, class HeavyID>
inline auto DoubleStore<Value, LightID, HeavyID>::getItem(HeavyID id) -> Value&
{
    return getItem(getItemIndex(id));
}

template<class Value, class LightID, class HeavyID>
inline const auto DoubleStore<Value, LightID, HeavyID>::getItem(HeavyID id) const -> const Value&
{
    return getItem(getItemIndex(id));
}

template<class Value, class LightID, class HeavyID>
auto DoubleStore<Value, LightID, HeavyID>::getItemIndex(const HeavyID& name) const -> LightID
{
    auto it = m_conversionTable.find(name);
    TealAssert(it != m_conversionTable.end(), "Item with that name does not exist !");

    return it->second;
}
