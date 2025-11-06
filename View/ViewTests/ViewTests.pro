TEMPLATE = app
CONFIG += console c++17 testcase
QT += testlib

TARGET = ViewTests
INCLUDEPATH += $$PWD/../include
INCLUDEPATH += $$PWD/../../lib/DesignLibrary/DesignPattern
DESTDIR = $$OUT_PWD

SOURCES += \
    tst_view.cpp

win32:LIBS += -L$$OUT_PWD/.. -lView
unix:LIBS += -L$$OUT_PWD/.. -lView

win32:CONFIG(debug, debug| release): LIBS += -L$$OUT_PWD/../../lib/DesignLibrary/DesignPattern/debug -lDesignPattern
else:win32:CONFIG(release, debug| release): LIBS += -L$$OUT_PWD/../../lib/DesignLibrary/DesignPattern/release -lDesignPattern
else:unix:LIBS += -L$$OUT_PWD/../../lib/DesignLibrary/DesignPattern -lDesignPattern


