// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef UIDEF_HPP
#define UIDEF_HPP

namespace Def
{

constexpr unsigned MAPXSIZE { 512u };
constexpr unsigned MAPYSIZE { 256u }; // 8 tiles * 32px

constexpr unsigned MAPXVIEWPORT { MAPXSIZE };
constexpr unsigned MAPYVIEWPORT { MAPYSIZE + 16u };

constexpr unsigned BUTTONSXSIZE { MAPXSIZE };
constexpr unsigned BUTTONSYSIZE { 90u };

constexpr unsigned WINXSIZE { MAPXSIZE };
constexpr unsigned WINYSIZE { MAPYSIZE + BUTTONSYSIZE };

}

#endif // UIDEF_HPP

