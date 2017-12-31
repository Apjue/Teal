// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef AICORE_HPP
#define AICORE_HPP

#include <Nazara/Core/String.hpp>
#include <utility>
#include <map>

class AICore
{
public:
    using TagKey = std::pair<Nz::String /*family*/, Nz::String /*monster name*/>;
    using TagInfo = std::pair<Nz::String /*filepath*/, unsigned /*tag power*/>;

    AICore() = default;
    ~AICore() = default;

    inline void addTag(const TagKey& tagKey, const TagInfo& tagInfo);
    inline bool hasTag(const TagKey& tagKey) const;
    inline const TagInfo& getTagInfo(const TagKey& tagKey) const;

    template<class... Ts>
    inline TagInfo getTagInfoFromTagKeys(const TagKey& tagKey, const Ts&... tagKeys) const;

private:
    using TagMap = std::map<TagKey, TagInfo>;
    TagMap m_tags;
    
    template<class... Ts>
    inline TagInfo getTagInfoFromTagKeys(const TagKey& tagKey, const Ts&... tagKeys, TagMap& tagMap) const;
    inline TagInfo getTagInfoFromTagKeys(TagMap& tagMap) const;
};

#include "aicore.inl"

#endif // AICORE_HPP
