TEMPLATE = lib
CONFIG += shared c++17 warn_on plugin
QT -= gui

TARGET = View
INCLUDEPATH += $$PWD/include
DESTDIR = $$OUT_PWD

HEADERS += \
    include/View/view.h

SOURCES += \
    src/view.cpp


INCLUDEPATH += $$PWD/../lib/DesignLibrary/DesignPattern

win32:CONFIG(debug, debug| release): LIBS += -L$$OUT_PWD/../lib/DesignLibrary/DesignPattern/debug -lDesignPattern
else:win32:CONFIG(release, debug| release): LIBS += -L$$OUT_PWD/../lib/DesignLibrary/DesignPattern/release -lDesignPattern
else:unix:LIBS += -L$$OUT_PWD/../lib/DesignLibrary/DesignPattern -lDesignPattern

