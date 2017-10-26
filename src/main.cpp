// Copyright (C) 2017 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Application.hpp>
#include <iostream>
#include "def/uidef.hpp"
#include "game.hpp"

#ifndef TEAL_DEBUG
#include <exception>
#endif

int main()
{
    Ndk::Application app;

    #ifndef TEAL_DEBUG
    try
    {
    #endif

        Game game { app, { Def::WindowSizeX, Def::WindowSizeY }, { Def::MapSizeX, Def::MapSizeY }, "TealDemo - Prototype" };
        // app.EnableFPSCounter(true);

        while (app.Run())
        {
            game.run();
        }

    #ifndef TEAL_DEBUG
    }

    catch (const std::exception& e)
    {
        std::cerr << "\n ===============\n" << "A problem occurred!\nError message: ";
        std::cerr << e.what() << "\n ===============\n" << std::endl;
    }
    #endif

    std::cout << "Goodbye." << std::endl;
    return 0;
}
