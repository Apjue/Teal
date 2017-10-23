// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Application.hpp>
#include "def/uidef.hpp"
#include "game.hpp"

int main()
{
    Ndk::Application app;

    Game game { app, { Def::WindowSizeX, Def::WindowSizeY },
                     { Def::MapSizeX, Def::MapSizeY }, "TealDemo - Prototype" };

    // app.EnableFPSCounter(true);
    
    while (app.Run())
    {
        game.run();
    }

    return 0;
}
