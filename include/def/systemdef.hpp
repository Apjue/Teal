// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef SYSTEMDEF_HPP
#define SYSTEMDEF_HPP

namespace Def
{

constexpr int RandomMovementSystemUpdateOrder { 1 };
constexpr int AISystemUpdateOrder { RandomMovementSystemUpdateOrder + 1 };
constexpr int MovementSystemUpdateOrder { AISystemUpdateOrder + 1 };
constexpr int AnimationSystemUpdateOrder { MovementSystemUpdateOrder + 1 };
constexpr int FightSystemUpdateOrder { AnimationSystemUpdateOrder + 1 };

}

#endif // SYSTEMDEF_HPP
