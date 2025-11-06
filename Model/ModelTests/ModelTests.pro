TEMPLATE = app
CONFIG += console c++17 testcase
QT += testlib

TARGET = ModelTests
INCLUDEPATH += $$PWD/../include
DESTDIR = $$OUT_PWD

SOURCES += \
    tst_model.cpp

win32:LIBS += -L$$OUT_PWD/.. -lModel
unix:LIBS += -L$$OUT_PWD/.. -lModel


