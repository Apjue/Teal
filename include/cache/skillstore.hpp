// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef SKILLSTORE_HPP
#define SKILLSTORE_HPP

#include <Nazara/Core/String.hpp>
#include <unordered_map>
#include <vector>
#include "data/skilldata.hpp"

class SkillStore
{
public:
    using ID = unsigned;

    SkillStore() = default;
    ~SkillStore() = default;

    inline ID addSkill(const Nz::String& name, const SkillData& skill);
    inline SkillData getSkill(ID id);
    inline ID getSkillIndex(const Nz::String& name);

private:
    ID counter { 0u };
    std::vector<SkillData> m_skills;
    std::unordered_map<Nz::String, ID> m_conversionTable;
};

#include "skillstore.inl"

#endif // SKILLSTORE_HPP
