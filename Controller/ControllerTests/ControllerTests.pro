TEMPLATE = app
CONFIG += console c++17 testcase
QT += testlib

TARGET = ControllerTests
INCLUDEPATH += $$PWD/../include
DESTDIR = $$OUT_PWD

SOURCES += \
    tst_controller.cpp

win32:LIBS += -L$$OUT_PWD/.. -lController
unix:LIBS += -L$$OUT_PWD/.. -lController


