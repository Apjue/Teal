// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_LIVINGENTITYUTIL_HPP
#define TEAL_LIVINGENTITYUTIL_HPP

#include <NDK/Entity.hpp>
#include "components/shared/damagemodifiercomponent.hpp"

inline DamageModifierList getLivingEntityDamageModifier(const Ndk::EntityHandle& e);

#include "livingentityutil.inl"

#endif // TEAL_LIVINGENTITYUTIL_HPP
