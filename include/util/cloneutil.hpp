// Copyright (C) 2017 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CLONEUTIL_HPP
#define CLONEUTIL_HPP

#include <NDK/Entity.hpp>
#include <NDK/EntityList.hpp>
#include <Nazara/Core/String.hpp>

Ndk::EntityHandle cloneCharacter(const Ndk::EntityList& characters, const Nz::String& codename);
Ndk::EntityHandle cloneItem(const Ndk::EntityList& items, const Nz::String& codename);

#endif // CLONEUTIL_HPP
