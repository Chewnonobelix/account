TEMPLATE = lib
CONFIG += shared c++17 warn_on plugin
QT -= gui

TARGET = Controller
DEFINES += CONTROLLER_LIBRARY
INCLUDEPATH += $$PWD/include
DESTDIR = $$OUT_PWD

HEADERS += \
    include/Controller/abstractdatastorage.h \
    include/Controller/abstractstoragelayer.h \
    include/Controller/controller.h \
    include/Controller/controller_global.h \
    include/Controller/inmemorystoragelayer.h \
    include/Controller/storagecommand.h

SOURCES += \
    src/abstractdatastorage.cpp \
    src/controller.cpp \
    src/inmemorystoragelayer.cpp
