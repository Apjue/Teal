// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Application.hpp>
#include <NDK/StateMachine.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Core/CallOnExit.hpp>
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
    Nz::CallOnExit guiltCallback { []() { std::cout << "why did you kill me :(" << std::endl; } };

    Ndk::Application app;
    Ndk::World& world = app.AddWorld();
    Nz::RenderWindow& window = app.AddWindow<Nz::RenderWindow>();

    window.Create(Nz::VideoMode(Def::WindowSizeX, Def::WindowSizeY, 32), "Teal - Prototype");
    window.EnableVerticalSync(true);

    GameData gameData;
    gameData.world = world.CreateHandle();

    initializeTeal(window, gameData);
    Nz::CallOnExit destroyerCallback { [&gameData]() { uninitializeTeal(gameData); } };

    Ndk::StateMachine fsm (std::make_shared<GameState>(window, Nz::Vector2ui { Def::MapSizeX, Def::MapSizeY }, gameData));

    #ifndef TEAL_DEBUG
    try
    {
    #endif

        while (app.Run())
        {
            if (!fsm.Update(app.GetUpdateTime()))
            {
                // Panic
                std::cerr << "\nCannot update State Machine, closing Teal..." << std::endl;

                return EXIT_FAILURE;
            }

            window.Display();
        }

    #ifndef TEAL_DEBUG
    }

    catch (const std::exception& e)
    {
        std::cerr << "\n =============== \n"
                  << "A problem occurred!"
                  << "\nError message: " << e.what()
                  << "\n =============== \n"
                  << std::flush;

        return EXIT_FAILURE;
    }
    #endif

    return EXIT_SUCCESS;
}
