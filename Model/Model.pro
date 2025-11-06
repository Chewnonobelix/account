TEMPLATE = lib
CONFIG += shared c++17 warn_on
QT -= gui

TARGET = Model
INCLUDEPATH += $$PWD/include
DESTDIR = $$OUT_PWD

HEADERS += \
    include/Model/model.h

SOURCES += \
    src/model.cpp


