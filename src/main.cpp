// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <QApplication>
#include <stdexcept>
#include <iostream>
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("./");
    QApplication app(argc, argv);

    MainWindow w{nullptr, {Def::MAPXSIZE, Def::MAPYSIZE}, {Def::BUTTONSXSIZE, Def::MAPYSIZE+Def::BUTTONSYSIZE}};
    w.setWindowTitle("Tealdemo - Pre-Prototype");
    w.setWindowIcon(QIcon(":/game/money"));
    w.show();

    int result {};

    try
    {
        result = app.exec();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception caught. Content:" << std::endl
                  << e.what() << std::endl << "Abort." << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        std::cerr << "Something caught. Abort." << std::endl;
        return EXIT_FAILURE;
    }


    return result;
}
