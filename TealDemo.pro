# Copyright (C) 2016 Samy Bensaid
# This file is part of the Teal game.
# For conditions of distribution and use, see copyright notice in LICENSE

TEMPLATE = app
QT = widgets
TARGET = TealDemo
CONFIG += c++14 warn_on

LIBS += -LC:/Qt/5.6/anax/lib -LC:/Qt/5.6/micropather/lib

#CONFIG(debug): LIBS += -lanax_d.dll -lmpd
#CONFIG(release): LIBS += -lanax.dll -lmp
debug
{
    LIBS += -lanax_d.dll -lmpd
    LIBS -= -lanax.dll -lmp
}

release
{
    LIBS += -lanax.dll -lmp
    LIBS -= -lanax_d.dll -lmpd
}

INCLUDEPATH += C:/Qt/5.6/anax/include C:/Qt/5.6/micropather/include $$_PRO_FILE_PWD_/include
DEPENDPATH += C:/Qt/5.6/anax/include C:/Qt/5.6/micropather/include $$_PRO_FILE_PWD_/include

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/graphicsscene.cpp \
    src/buttons.cpp \
    src/systems.cpp \
    src/global.cpp \
    src/factory.cpp \
    src/util.cpp \
    src/components.cpp \
    src/event.cpp

HEADERS += \
    include/mainwindow.hpp \
    include/graphicsscene.hpp \
    include/components.hpp \
    include/util.hpp \
    include/systems.hpp \
    include/buttons.hpp \
    include/events.hpp \
    include/global.hpp \
    include/event.hpp \
    include/factory.hpp \
    include/basesystems.hpp \
    include/chrono.hpp \
    include/cache.hpp \
    include/characterinfos.hpp \
    include/vector2.hpp \
    include/components/common.hpp \
    include/components/items.hpp

RESOURCES += res.qrc
