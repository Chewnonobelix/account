TEMPLATE = app
CONFIG += c++17 console
QT += core

TARGET = OpenAccount
DESTDIR = $$OUT_PWD

SOURCES += \
    src/main.cpp

# Inclure les en-têtes des modules
INCLUDEPATH += \
    $$PWD/../../Controller/include \
    $$PWD/../../Model/include \
    $$PWD/../../View/include \
    $$PWD/../../lib/DesignLibrary/DesignPattern

# Lier contre les bibliothèques construites par les sous-projets
win32:LIBS += \
    -L$$OUT_PWD/../../Controller -lController \
    -L$$OUT_PWD/../../Model -lModel \
    -L$$OUT_PWD/../../View -lView
unix:LIBS += \
    -L$$OUT_PWD/../../Controller -lController \
    -L$$OUT_PWD/../../Model -lModel \
    -L$$OUT_PWD/../../View -lView

win32:CONFIG(debug, debug| release): LIBS += -L$$OUT_PWD/../../lib/DesignLibrary/DesignPattern/debug -lDesignPattern
else:win32:CONFIG(release, debug| release): LIBS += -L$$OUT_PWD/../../lib/DesignLibrary/DesignPattern/release -lDesignPattern
else:unix:LIBS += -L$$OUT_PWD/../../lib/DesignLibrary/DesignPattern -lDesignPattern


