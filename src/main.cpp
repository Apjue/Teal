// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Application.hpp>
#include <stdexcept>
#include "game.hpp"

int main()
{
    Ndk::Application app;

    Game window { app, {Def::BUTTONSXSIZE, Def::MAPYSIZE+Def::BUTTONSYSIZE}, 
        "TealDemo - Prototype" /*, { Def::MAPXSIZE, Def::MAPYSIZE }*/ };

    while (app.Run())
    {
        window.update();
    }
    return 0;
}
