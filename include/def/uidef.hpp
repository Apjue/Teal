// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UIDEF_HPP
#define UIDEF_HPP

#include "gamedef.hpp"

namespace Def
{

constexpr unsigned MAPXSIZE { Def::REALMAPX * Def::TILEXSIZE }; // COORDFIX_REDO
constexpr unsigned MAPYSIZE { Def::REALMAPY * Def::TILEYSIZE };

constexpr unsigned MAPXVIEWPORT { MAPXSIZE };
constexpr unsigned MAPYVIEWPORT { MAPYSIZE + 16u };

constexpr unsigned BUTTONSXSIZE { MAPXSIZE };
constexpr unsigned BUTTONSYSIZE { 90u };

constexpr unsigned WINXSIZE { MAPXVIEWPORT };
constexpr unsigned WINYSIZE { MAPYVIEWPORT + BUTTONSYSIZE };

}

#endif // UIDEF_HPP

