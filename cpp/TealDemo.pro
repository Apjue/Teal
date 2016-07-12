# Copyright (C) 2016 Samy Bensaid
# This file is part of the Teal game.
# For conditions of distribution and use, see copyright notice in LICENSE

TEMPLATE = app
QT = widgets
TARGET = TealDemo
CONFIG += c++14 warn_on

LIBS += -LC:/Qt/5.6/anax/lib -LC:/Qt/5.6/micropather/lib

CONFIG(debug): LIBS += -lanax_d.dll -lmpd
CONFIG(release): LIBS += -lanax.dll -lmp

INCLUDEPATH += C:/Qt/5.6/anax/include C:/Qt/5.6/micropather/include
DEPENDPATH += C:/Qt/5.6/anax/include C:/Qt/5.6/micropather/include

SOURCES += main.cpp \
    mainwindow.cpp \
    graphicsscene.cpp \
    buttons.cpp \
    systems.cpp \
    global.cpp

HEADERS += \
    mainwindow.h \
    graphicsscene.h \
    components.hpp \
    util.hpp \
    systems.hpp \
    buttons.h \
    events.hpp \
    global.hpp \
    event.hpp \
    factory.hpp \
    basesystems.hpp

RESOURCES += res.qrc
