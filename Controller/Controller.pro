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


INCLUDEPATH += $$PWD/../lib/DesignLibrary/DesignPattern

win32:CONFIG(debug, debug| release): LIBS += -L$$OUT_PWD/../lib/DesignLibrary/DesignPattern/debug -lDesignPattern
else:win32:CONFIG(release, debug| release): LIBS += -L$$OUT_PWD/../lib/DesignLibrary/DesignPattern/release -lDesignPattern
else:unix:LIBS += -L$$OUT_PWD/../lib/DesignLibrary/DesignPattern -lDesignPattern

