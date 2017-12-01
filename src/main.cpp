// Copyright (C) 2017 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Application.hpp>
#include <NDK/StateMachine.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <iostream>
#include <memory>
#include "def/uidef.hpp"
#include "init/tealinit.hpp"
#include "states/gamestate.hpp"

#ifndef TEAL_DEBUG
#include <exception>
#endif

int main()
{
    Ndk::Application app;
    Ndk::World& world = app.AddWorld();
    Nz::RenderWindow& window = app.AddWindow<Nz::RenderWindow>();

    window.Create(Nz::VideoMode(Def::WindowSizeX, Def::WindowSizeY, 32), "TealDemo - Prototype");
    window.EnableVerticalSync(true);

    GameData gameData;
    initializeTeal(world, window, gameData);

    Ndk::StateMachine fsm (std::make_shared<GameState>(world.CreateHandle(), window, Nz::Vector2ui { Def::MapSizeX, Def::MapSizeY }, gameData));

    #ifndef TEAL_DEBUG
    try
    {
    #endif

        while (app.Run())
        {
            if (!fsm.Update(app.GetUpdateTime()))
            {
                // Panic
                std::cerr << "\nCannot update State Machine, closing program..." << std::endl;

                return EXIT_FAILURE;
            }

            window.Display();
        }

    #ifndef TEAL_DEBUG
    }

    catch (const std::exception& e)
    {
        std::cerr << "\n ===============\n" << "A problem occurred!\nError message: ";
        std::cerr << e.what() << "\n ===============\n" << std::endl;
        return EXIT_FAILURE;
    }
    #endif

    std::cout << "Goodbye." << std::endl;
    return EXIT_SUCCESS;
}
