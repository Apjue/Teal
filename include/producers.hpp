// Copyright (C) 2016 Samy Bensaid
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef PRODUCERS_HPP
#define PRODUCERS_HPP

#include <memory>
#include <Nazara/Core/ObjectRef.hpp>
#include <Nazara/Core/ObjectHandle.hpp>

// These producers are mainly used by the Cache class
// But they can also be used without it !

template<class T>
struct SharedPointerProducer
{
    template<class... Args>
    static std::shared_ptr<T> create(Args&&... args)
    {
        std::shared_ptr<T> res = std::make_shared<T>(std::forward<Args>(args)...);
        return res;
    }
};

template<class T>
struct RawPointerProducer
{
    template<class... Args>
    static T* create(Args&&... args)
    {
        std::unique_ptr<T> res = std::make_unique<T>(std::forward<Args>(args)...);
        return res.release();
    }
};

template<class T>
struct NzObjectRefProducer
{
    template<class... Args>
    static Nz::ObjectRef<T> create(Args&&... args)
    {
        Nz::ObjectRef<T> res = T::New(std::forward<Args>(args)...);
        return res;
    }
};

template<class T>
struct NzObjectRefCopier
{
    template<class... Args>
    static Nz::ObjectRef<T> create(Args&&... args)
    {
        Nz::ObjectRef<T> res { std::forward<Args>(args)... };
        return res;
    }
};

template<class T>
struct NzObjectHandleProducer
{
    template<class... Args>
    static Nz::ObjectHandle<T> create(Args&&... args)
    {
        Nz::ObjectHandle<T> res(std::forward<Args>(args)...);
        return res;
    }
};

#endif // PRODUCERS_HPP