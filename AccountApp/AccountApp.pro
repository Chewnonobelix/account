CONFIG += c++20
QT += quick sql core xml widgets qml gui

TEMPLATE = app
TARGET = Account
SOURCES += \
    main.cpp

INCLUDEPATH += $$PWD/../Account

LIBS += -L$$OUT_PWD/ -lLibAccount


INCLUDEPATH += $$PWD/../DesignLibrary/DesignPattern
DEPENDPATH += $$OUT_PWD/../DesignLibrary/DesignPattern/
LIBS += -L$$OUT_PWD/../DesignLibrary/DesignPattern/ -lDesignPattern

zip.path = $$OUT_PWD
zip.files = $$PWD/../7z.dll $$PWD/../7z.exe

INSTALLS += zip
