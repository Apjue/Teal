// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ATTACKDATA_HPP
#define ATTACKDATA_HPP

struct AttackData
{
    enum class Target
    {
        Allies,
        Enemies,
        Both
    };

    Target target { Target::Enemies };
};

#endif // ATTACKDATA_HPP
