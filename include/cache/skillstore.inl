// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

SkillStore::ID SkillStore::addSkill(const Nz::String& name, const SkillData& skill)
{
    NazaraAssert(m_conversionTable.find(name) == m_conversionTable.end(), "Skill with that name already exists !");

    m_skills.push_back(skill);
    m_conversionTable[name] = counter;

    return counter++;
}

SkillData SkillStore::getSkill(ID id)
{
    return m_skills[id];
}

SkillStore::ID SkillStore::getSkillIndex(const Nz::String& name)
{
    NazaraAssert(m_conversionTable.find(name) != m_conversionTable.end(), "Skill with that name does not exist !");
    return m_conversionTable[name];
}
