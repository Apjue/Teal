// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Application.hpp>
#include <Nazara/Core/AbstractLogger.hpp>
#include <Nazara/Core/Log.hpp>
#include "game.hpp"

int main()
{
    Ndk::Application app;

//     Nz::Log::GetLogger()->EnableStdReplication(false);

    Game game { app, { Def::WINXSIZE, Def::WINYSIZE },
                     { Def::MAPXSIZE, Def::MAPYSIZE }, "TealDemo - Prototype" };
    
    while (app.Run())
    {
        game.run();
    }

    return 0;
}
