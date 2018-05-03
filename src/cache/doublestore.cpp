// Copyright (C) 2018 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "cache/doublestore.hpp"

template<class T>
std::shared_ptr<DoubleStore<T>> DoubleStores<T>::instance {};
