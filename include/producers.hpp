// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef PRODUCERS_HPP
#define PRODUCERS_HPP

#include <memory>
#include <Nazara/Core/ObjectRef.hpp>

template<class T>
struct SharedPointerProducer
{
    template<class... Args>
    static std::shared_ptr<T> create(Args&&... args)
    {
        std::shared_ptr res = std::make_shared<T>(std::forward<Args>(args)...);
        return res;
    }
};

template<class T>
struct RawPointerProducer
{
    template<class... Args>
    static T* create(Args&&... args)
    {
        std::unique_ptr res = std::make_unique<T>(std::forward<Args>(args)...);
        return res.release();
    }
};

template<class T>
struct ObjectRefProducer
{
    template<class... Args>
    static Nz::ObjectRef<T> create(Args&&... args)
    {
        Nz::ObjectRef<T> res = T::New(std::forward<Args>(args)...);
        return res;
    }
};

template<class T>
struct ObjectRefCopier
{
    template<class... Args>
    static Nz::ObjectRef<T> create(Args&&... args)
    {
        Nz::ObjectRef<T> res { std::forward<Args>(args)... };
        return res;
    }
};

#endif // PRODUCERS_HPP