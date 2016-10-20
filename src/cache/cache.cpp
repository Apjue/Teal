// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "cache/cache.hpp"

template<class Key, class T, class Producer>
typename Cache<Key, T, Producer>::ManagerType Cache<Key, T, Producer>::empty {};
