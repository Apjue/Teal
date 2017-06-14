// Copyright (C) 2017 Samy Bensaid
// This file is part of the  project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ASSERT_HPP
#define ASSERT_HPP

#ifdef TEAL_DEBUG
    #include <Nazara/Core/Error.hpp>
    #define TealAssert(a, err) if (!(a)) Nz::Error::Trigger(Nz::ErrorType_AssertFailed, err, __LINE__, __FILE__, NAZARA_FUNCTION)
#else
    #define TealAssert(a, err) for (;;) break
#endif

#endif // ASSERT_HPP
