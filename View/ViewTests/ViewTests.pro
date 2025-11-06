TEMPLATE = app
CONFIG += console c++17 testcase
QT += testlib

TARGET = ViewTests
INCLUDEPATH += $$PWD/../include
DESTDIR = $$OUT_PWD

SOURCES += \
    tst_view.cpp

win32:LIBS += -L$$OUT_PWD/.. -lView
unix:LIBS += -L$$OUT_PWD/.. -lView


