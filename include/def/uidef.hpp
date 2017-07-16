// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UIDEF_HPP
#define UIDEF_HPP

#include "gamedef.hpp"

namespace Def
{

constexpr unsigned MAPXSIZE { (Def::ARRAYMAPX) * Def::TILEXSIZE };
constexpr unsigned MAPYSIZE { (Def::ARRAYMAPY + 1u) / 2 * Def::TILEYSIZE + TILEYSIZE / 2 };

constexpr unsigned BUTTONSMARGINX { 10u };
constexpr unsigned BUTTONSMARGINY { 5u };

constexpr unsigned BUTTONSXSIZE { MAPXSIZE };
constexpr unsigned BUTTONSYSIZE { 90u };

constexpr unsigned WINXSIZE { MAPXSIZE };
constexpr unsigned WINYSIZE { MAPYSIZE + BUTTONSMARGINY + BUTTONSYSIZE };

}

#endif // UIDEF_HPP

