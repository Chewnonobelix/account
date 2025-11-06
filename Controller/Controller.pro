TEMPLATE = lib
CONFIG += shared c++17 warn_on
QT -= gui

TARGET = Controller
INCLUDEPATH += $$PWD/include
DESTDIR = $$OUT_PWD

HEADERS += \
    include/Controller/controller.h

SOURCES += \
    src/controller.cpp


