// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef ASSERT_HPP
#define ASSERT_HPP

#include <Nazara/Core/Error.hpp>
#include <Nazara/Core/String.hpp>
#include <stdexcept>

#ifdef TEAL_DEBUG
    #define TealAssert(a, err) if (!(a)) Nz::Error::Trigger(Nz::ErrorType_AssertFailed, err, __LINE__, __FILE__, NAZARA_FUNCTION)
#else
    #define TealAssert(a, err) for (;;) break
#endif

#define TealException(a, err) if (!(a)) throw std::runtime_error { Nz::String { Nz::String { err } + Nz::String { " File: " } + Nz::String { __FILE__ } + Nz::String { ", line: " } + \
                                                                   Nz::String::Number(__LINE__) + Nz::String { ", function: " } + NAZARA_FUNCTION }.ToStdString() }

#endif // ASSERT_HPP
