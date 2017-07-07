Coding Style examples:

Class Header:

```cpp
// Copyright (C) YEAR AUTHOR
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef CLASSNAME_HPP
#define CLASSNAME_HPP

#include <NDK/Header.hpp>
#include <Nazara/Module/Header.hpp>
#include <STLHeader>
#include "myheader.hpp"

class MyClass : public AnotherClass
{
public:
    DefaultConstructor() = default;

    Constructors();

    ~DefaultDestructor() = default;
    ~Destructor(); // Only if required, else use default destructor

    functionA(unsigned a, unsigned long b, int short c);
    virtual functionB(const unsigned& a, const unsigned long* b) const override;

private:
    PrimitiveType m_varA {};
    PrimitiveType m_varB { 58ul };
    STLType m_varC;
    OtherType m_varD;
    SomeType* m_varE {};
};

#endif // CLASSNAME_HPP
```

Class Source:
```cpp
// Copyright (C) YEAR AUTHOR
// This file is part of the TealDemo project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "myclass.hpp"

MyClass::functionB(const unsigned& a, const unsigned long* b) const
{
    for (int i { 4 }; i < 10; ++i);

    const int c;
    const unsigned& d { a };

    // ...
}

```

Doxygen Block:
```cpp
///
/// \class SomeClass
///
/// \brief blah blah blah
///        bloh bloh bloh
///
/// \note bluh bluh bluh
///
/// \todo do a correct coding style file
///
```

Enum:
```cpp
enum a
{
    Undefined, // Not required
    W0t,
    Hello_World,
    Nope
};

enum b
{
    Hello = 0,
    SomeFlag = 1 << 0,
    AnotherOne = 1 << 1
};
```
