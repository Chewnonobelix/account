TEMPLATE = app
CONFIG += console c++17 testcase
QT += testlib

TARGET = StorageLayerTests
INCLUDEPATH += $$PWD/../include
DESTDIR = $$OUT_PWD

SOURCES += \
    tst_storagelayer.cpp

win32:LIBS += -L$$OUT_PWD/.. -lController
unix:LIBS += -L$$OUT_PWD/.. -lController
