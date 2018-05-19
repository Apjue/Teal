// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "util/assert.hpp"
#include "aicore.hpp"

inline void AICore::addTag(const TagKey& tagKey, const TagInfo& tagInfo)
{
    m_tags[tagKey] = tagInfo;
}

inline bool AICore::hasTag(const TagKey& tagKey) const
{
    auto it = m_tags.find(tagKey);
    return it != m_tags.end();
}

inline const AICore::TagInfo& AICore::getTagInfo(const TagKey& tagKey) const
{
    TealAssert(m_tags.find(tagKey) != m_tags.end(), "Tag not found...");

    auto it = m_tags.find(tagKey);
    return it->second;
}

template<class... Ts>
inline AICore::TagInfo AICore::getTagInfoFromTagKeys(const TagKey& tagKey, const Ts&... tagKeys) const
{
    TagMap tagMap;

    if (hasTag(tagKey))
        tagMap[tagKey] = getTagInfo(tagKey);

    return getTagInfoFromTagKeys(std::forward<Ts>(tagKeys)..., tagMap);
}

template<class... Ts>
inline AICore::TagInfo AICore::getTagInfoFromTagKeys(const TagKey& tagKey, const Ts&... tagKeys, TagMap& tagMap) const
{
    if (hasTag(tagKey))
        tagMap[tagKey] = getTagInfo(tagKey);

    return getTagInfoFromTagKeys(std::forward<Ts>(tagKeys)..., tagMap);
}

inline AICore::TagInfo AICore::getTagInfoFromTagKeys(TagMap& tagMap) const
{
    if (tagMap.empty() && hasTag(DefaultAI)) // Default AI
        return getTagInfo(DefaultAI);

    unsigned maxPower {};
    Nz::String filepath;

    for (auto& pair : tagMap)
    {
        if (pair.second.second > maxPower)
        {
            filepath = pair.second.first;
            maxPower = pair.second.second;
        }
    }

    return std::make_pair(filepath, maxPower);
}
