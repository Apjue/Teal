// Copyright (C) 2017 Samy Bensaid
// This file is part of the Tiled2Teal project.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <Nazara/Core/Initializer.hpp>
#include <Nazara/Core/Log.hpp>
#include <Nazara/Lua.hpp>
#include <iostream>
#include <algorithm>
#include <regex>
#include "util/assert.hpp"
#include "util.hpp"
#include "parser.hpp"

bool showMenu(std::ostream& out = std::cout, std::istream& in = std::cin); // false = quit

int main()
{
    Nz::Initializer<Nz::Lua> nazara;

    if (!nazara)
    {
        std::cerr << "Nazara failed to initialize correctly !" << std::endl;
        return EXIT_FAILURE;
    }

    Nz::String version { "0.1p" };
    std::cout << "Tiled2Teal version " << version << std::endl;
    std::cout << " -------------------- \n" << std::endl;

    try
    {
        initTilesetCore();
    }

    catch (const std::exception& e)
    {
        std::cout << "Fatal error ! TilesetCore initialization failed." << std::endl;
        std::cout << e.what() << std::endl;

        return EXIT_FAILURE;
    }

    for (;;)
    {
        bool choice = showMenu();

        if (!choice)
        {
            std::cout << "Exiting...\n" << std::endl;
            break;
        }

        else
        {
            bool ok { true };

            try
            {
                Nz::String tex;
                Nz::String ftex;
                Nz::String inv;
                Nz::String obs;
                Nz::String spawn;

                std::cout << "Filepath of textures csv: " << std::flush;
                std::getline(std::cin, tex);

                std::cout << "Filepath of fight textures csv: " << std::flush;
                std::getline(std::cin, ftex);

                std::cout << "Filepath of invisibility layer csv: " << std::flush;
                std::getline(std::cin, inv);

                std::cout << "Filepath of obstacle layer csv: " << std::flush;
                std::getline(std::cin, obs);

                std::cout << "Filepath of spawn layer csv: " << std::flush;
                std::getline(std::cin, spawn);

                Nz::String teal;
                Nz::String pos;

                std::cout << "Filepath of Teal map (file will be overwritten): " << std::flush;
                std::getline(std::cin, teal);

                std::cout << "Position of map (x;y): " << std::flush;
                std::getline(std::cin, pos);

                std::string posStd = pos.ToStdString();
                std::regex posRegex("^-?[[:digit:]]+;-?[[:digit:]]+$");

                if (!std::regex_search(posStd, posRegex))
                    std::cout << "Wrong position. Format must be x;x" << std::endl;

                else
                    convertTiledMap(tex, ftex, inv, obs, spawn, teal, pos);
            }

            catch (const std::exception& e)
            {
                std::cout << "Exception caught !" << std::endl;
                std::cout << e.what() << std::endl;

                ok = false;
            }

            std::cout << "Converting has " << (ok ? "succeeded !" : "failed.") << '\n' << std::endl;
        }
    }

    return 0;
}

bool showMenu(std::ostream& out, std::istream& in)
{
    out << "Convert a tiled map into a teal map ? [y/n] " << std::flush;
    
    std::string choice;
    bool ok {};

    while (!ok)
    {
        if (safeAnswer(in, choice))
        {
            std::transform(choice.begin(), choice.end(), choice.begin(), ::tolower);

            if (choice == "n")
            {
                out << "Exiting..." << std::endl;
                return false;
            }

            else if (choice == "y")
                ok = true;

            else
                out << "Please insert y or n" << std::endl;
        }

        else
            out << "Only y or n are allowed." << std::endl;

        if (!ok)
            out << "Convert a tiled map into a teal map ? [y/n] " << std::flush;
    }

    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    out << "Data required:" << std::endl;

    return true;
}
