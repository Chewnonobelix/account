TEMPLATE = lib
CONFIG += shared c++17 warn_on
QT -= gui

TARGET = View
INCLUDEPATH += $$PWD/include
DESTDIR = $$OUT_PWD

HEADERS += \
    include/View/view.h

SOURCES += \
    src/view.cpp


