// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Application.hpp>
#include "game.hpp"
#include "def/uidef.hpp"

int main()
{
    Nz::Initializer<Ndk::Sdk> sdk;
    Ndk::Application app;

    Game game { app, { Def::WINXSIZE, Def::WINYSIZE },
                     { Def::MAPXVIEWPORT, Def::MAPYVIEWPORT }, "TealDemo - Prototype" };

    // app.EnableFPSCounter(true);
    
    while (app.Run())
    {
        game.run();
    }

    return 0;
}
