// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <QApplication>
#include <stdexcept>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include "mainwindow.hpp"

namespace
{

QFile outputFile{};

}

void outputToFile(QtMsgType type, const QMessageLogContext& /*infos*/, const QString& msg)
{
    outputFile.write("[");
    outputFile.write(qPrintable(QDateTime::currentDateTime().toString()));
    outputFile.write("]");

    switch (type)
    {
    case QtInfoMsg:
        outputFile.write("[INFO] ");
        break;

    case QtDebugMsg:
        outputFile.write("[DEBUG] ");
        break;

    case QtWarningMsg:
        outputFile.write("[WARNING] ");
        break;

    case QtCriticalMsg:
        outputFile.write("[CRITICAL] ");
        break;

    case QtFatalMsg:
        outputFile.write("[FATAL] ");
        break;
    }

    outputFile.write(qPrintable(msg));
    outputFile.write("\r\n");
}

int main(int argc, char *argv[])
{

    QCoreApplication::addLibraryPath("./");
    QApplication app(argc, argv);

    outputFile.setFileName(QCoreApplication::applicationDirPath()+"/output.log");
    bool outToFile = outputFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    qDebug();

    if(!QApplication::arguments().contains("--dev") && outToFile)
        qInstallMessageHandler(outputToFile);

    MainWindow w{nullptr, {Def::MAPXSIZE, Def::MAPYSIZE}, {Def::BUTTONSXSIZE, Def::MAPYSIZE+Def::BUTTONSYSIZE}};
    w.setWindowTitle("Tealdemo - Pre-Prototype");
    w.setWindowIcon(QIcon(":/game/money"));
    w.show();

    int result{};

    try
    {
        result = app.exec();
    }
    catch (const std::exception& e)
    {
        qCritical() << "Exception caught. Content:";
        qCritical() << e.what();
        qCritical() << "Abort";

        return EXIT_FAILURE;
    }
    catch (...)
    {
        qCritical() << "Something caught. Abort.";

        return EXIT_FAILURE;
    }

    return result;
}
